#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER "192.168.0.1"
#define BUFLEN 512
#define PORT 2015

void kill(char* string){
    printf("ERORRE: %s\n",string);
    perror(string);
    exit(1);
}

int main(void){
    struct sockaddr_in sockaddress;
    int ret, i, len = sizeof(sockaddress);
    char buf[BUFLEN];
    char message[BUFLEN];

    if((ret = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) kill("socket");

    int broadcast = 1;
    if(setsockopt(ret, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) kill("setsockopt");

    memset((char*) &sockaddress, 0, sizeof(sockaddress));
    sockaddress.sin_family = AF_INET;
    sockaddress.sin_port = htons(PORT);

    if(inet_aton(SERVER, &sockaddress.sin_addr) == 0)kill("inet_aton");

    while(1){
        //printf("Scrivi: \n");
        strcpy(message, "hello"); // Add "hello" message to message variable
        // fgets(message, BUFLEN, stdin); // Commented out to use the hardcoded message
        if(sendto(ret, message, strlen(message), 0, (struct sockaddr*) &sockaddress, len) == -1) kill("send to");
    }
    close(ret);

    return 0;
}

