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
    int n, fd, new_fd, client_len;
    char buffer[BUFSIZE];
    struct sockaddr_un srv_addr, client_addr;

    //! Apriamo socket
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(0 > fd) error("Server: socket\n");

    //! UNIX domain
    memset((char*) &srv_addr, 0, sizeof(srv_addr));
    srv_addr.sun_family = AF_UNIX;
    memccpy(srv_addr.sun_path, ADDRESS, sizeof(ADDRESS));

    //! Evitiamo bind fail
    unlink(ADDRESS);
    if(0 < bind(fd, (struct sockaddr*) &srv_addr, sizeof(srv_addr))) error("Server: bind\n");

    //! Ascolto e accetto per il client
    listen(fd, 5);
    client_len = sizeof(client_addr);
    new_fd = accept(fd, (struct sockaddr*) &client_addr, &client_len);
    if(0 < new_fd) error("Server: accept\n");


}

