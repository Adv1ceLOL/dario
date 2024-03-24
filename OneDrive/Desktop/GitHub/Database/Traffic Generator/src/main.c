#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <unistd.h>

#define PORT 8081
#define MAX_PACKETS 10

typedef struct Payload {
    int value;
    char* originator; //Per ricordare l'originator
    int sequenceNumber;
} Payload;

typedef struct Message {
    int source;
    int sequence;
    int payload_lenght;
    Payload * payload;
} Message;



Message * prepare_message(Payload * payload){
    Message* messaggio = (Message*) malloc(sizeof(Message));
    messaggio->payload = payload;
    return messaggio;
}

//---------------
// Traffic Generator
//---------------

// Some sistem that triggers handler callback every x milliseconds

//---------------
// Broadcaster
//---------------

typedef void (*BroadcasterHandler)(void * broadcaster, Payload * payload);

typedef struct Node {
    struct sockaddr_in address;
    int id;
} Node;

typedef struct Broadcaster {
    // .. Local data to manage broadcast
    // e.g. socket opened, network interfaces, 
    // sequence of other nodes...
    int socket;
    struct sockaddr_in interface;
    Node* nodes;
    int nodeCount;
    BroadcasterHandler handler;
} Broadcaster;



void initializeBroadcaster(Broadcaster* broadcaster, int port) {

    // Create socket
    broadcaster->socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (broadcaster->socket == -1) {
        perror("socket");
        exit(1);
    }

    // Enable broadcast
    int broadcastEnable = 1;
    int ret = setsockopt(broadcaster->socket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    if (ret) {
        perror("setsockopt");
        exit(1);
    }
}

void register_handler(Broadcaster * broadcaster, BroadcasterHandler handler) {
    broadcaster->handler = handler;
}

void sendUDP(Broadcaster * broadcaster, Payload * payload) {
    //!Creo il pacchetto
    char packet[sizeof(Payload)];
    memcpy(packet, payload, sizeof(payload));

    //! Invio il pacchetto
    for(int i = 0; i< broadcaster->nodeCount; i++){
        Node* node = &broadcaster->nodes[i];
        sendto(broadcaster->socket, packet, sizeof(packet), 0,
         (struct sockaddr*) &node->address, sizeof(node->address));
    }

}
//! Fatto una linked list per i pacchetti
typedef struct PacketNode {
    char packet[sizeof(Payload)];
    struct PacketNode *next;
} PacketNode;


PacketNode *seenPackets = NULL;

bool hasSeenPacket(char *packet, int packetSize) {
    PacketNode *current = seenPackets;
    while (current != NULL) {
        if (memcmp(current->packet, packet, packetSize) == 0) {

            return true;
        }
        current = current->next;
    }

    return false;
}

Payload receivedPackets[10];

void markPacketAsReceived(char* originator, int sequenceNumber) {
    for (int i = 0; i < 10; i++) {
        if (receivedPackets[i].originator == NULL) {
            receivedPackets[i].originator = strdup(originator);
            receivedPackets[i].sequenceNumber = sequenceNumber;
            break;
        }
    }
}

void broadcastPacket(Broadcaster* broadcaster, Payload* packet) {
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET)
            continue;

        if (setsockopt(broadcaster->socket, SOL_SOCKET, SO_BINDTODEVICE, ifa->ifa_name, strlen(ifa->ifa_name)) == -1) {
            perror("setsockopt");
            continue;
        }

        // Send Pacchetto
        struct sockaddr_in broadcastAddr;
        memset(&broadcastAddr, 0, sizeof(broadcastAddr));
        broadcastAddr.sin_family = AF_INET;
        broadcastAddr.sin_port = htons(PORT);
        broadcastAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

        if (sendto(broadcaster->socket, packet, sizeof(Payload), 0, (struct sockaddr*)&broadcastAddr, sizeof(broadcastAddr)) == -1) {
            perror("sendto");
        }
    }

    freeifaddrs(ifaddr);
}

void process_broadcaster(Broadcaster * broadcaster) {
    // In case of packet reception, notify the handler
    // Discard already seen packets
    //! Metto in storage il pacchetto ricevuto
    char buffer[sizeof(Payload)];

    //! Ricevo pacchetto
    struct sockaddr_in senderAddress;
    socklen_t senderAddressLenght = sizeof(senderAddress);
    int recivedBytes = recvfrom(broadcaster->socket, buffer, sizeof(buffer), 0, 
                                 (struct sockaddr *) &senderAddress, &senderAddressLenght);
    
    if(recivedBytes > 0){
        if(!hasSeenPacket(buffer,recivedBytes)) broadcaster->handler(broadcaster, (Payload*)buffer);
    }

}

//---------------
// Traffic Analyzer
//---------------

typedef struct TrafficAnalyzer {

    // Definition of sliding window...

} TrafficAnalyzer;

void received_pkt(TrafficAnalyzer * analyzer, int source) {
    // Record the packet send and datetime of it
}

void dump(TrafficAnalyzer * analyzer) {
    // Dump information about the thoughput of all packets received
}

//-------------------------
// Utility
// ------------------------

/**
 * Bind the given socket to all interfaces (one by one)
 * and invoke the handler with same parameter
 */
void bind_to_all_interfaces(int sock, void * context, void (*handler)(int, void *)) {
    struct ifaddrs *addrs, *tmp;
    getifaddrs(&addrs);
    tmp = addrs;
    while (tmp){
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET) {
            setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, tmp->ifa_name, sizeof(tmp->ifa_name));
            handler(sock, context);
        }
        tmp = tmp->ifa_next;
    }
    freeifaddrs(addrs);
}

/**
 * Sleep a given amount of milliseconds
 */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do
    {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void cleanupBroadcaster(Broadcaster* broadcaster) {
    // Close the socket
    if (broadcaster->socket != -1) {
        close(broadcaster->socket);
    }
}


int main() {

    setvbuf(stdout, NULL, _IONBF, 0);

    // Inizializzo Broadcaster
    Broadcaster broadcaster;
    initializeBroadcaster(&broadcaster, PORT);

    // Traffic generator
    time_t start_time = time(NULL);
    while (time(NULL) - start_time < 20) {
        Payload packet;
        sendUDP(&broadcaster,&packet); 
        broadcastPacket(&broadcaster, &packet); 
        markPacketAsReceived(packet.originator, packet.sequenceNumber);
        msleep(1000); 
    }

    // Traffic analyzer

    //Cleanup
    cleanupBroadcaster(&broadcaster);

}


