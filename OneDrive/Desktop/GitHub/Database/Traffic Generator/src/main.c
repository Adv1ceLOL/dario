#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdbool.h>

typedef struct Payload {
    int value;
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

void register_handler(Broadcaster * broadcaster, BroadcasterHandler handler) {
    broadcaster->handler = handler;
}

void sendUDP(Broadcaster * broadcaster, Payload * payload) {
    // Send UDP packet and update local sequence
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
            // The packet has been seen before
            return true;
        }
        current = current->next;
    }

    // The packet has not been seen before
    return false;
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


int main() {

    // Autoflush stdout for docker
    setvbuf(stdout, NULL, _IONBF, 0);

    // Traffic generator
    Broadcaster* broadcast;
    time_t start_time = time(NULL);
    while (time(NULL) - start_time < 20) {
        Payload packet;
        prepare_message(&packet); // Generate a new packet
        sendUDP(&broadcast,&packet); // Send the packet
        msleep(1000); // Wait for a while before sending the next packet
    }

    // Broadcaster

    // Traffic analyzer

}


