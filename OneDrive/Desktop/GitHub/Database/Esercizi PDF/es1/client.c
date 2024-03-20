#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define BUFSIZE 256
#define ADDRESS "socket_unix_IPC_example"

void error(char* string){
    perror(string);
    exit(1);
}


int main(){
int n, fd;
char buffer[BUFSIZE];
struct sockaddr_un srv_addr;

//! Apriamo il socket
fd = socket(AF_UNIX, SOCK_STREAM,0);
if(0 > fd) error("Client: socket\n");

//! UNIX domain
memset((char*) &srv_addr, 0, sizeof(srv_addr));
srv_addr.sun_family = AF_UNIX;
memcpy(srv_addr.sun_path, ADDRESS, sizeof(ADDRESS));

//! Connect
if(0 > connect(fd, (struct sockaddr*) &srv_addr, sizeof(srv_addr))) error("Client: connect\n");

//! Inviamo Qualcosa al server
printf("Started\n");
for(int i=5; i>0; i--){
    printf("%d...\n",i);

    //? Scriviamolo nel buffer
    memset(buffer, '\0', strlen(buffer));
    snprintf(buffer, BUFSIZE-1, "%d",i);
    //riga 30
}







}


