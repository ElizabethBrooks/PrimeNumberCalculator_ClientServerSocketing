/*
Name: Elizabeth Brooks 
File: primeCalculator_server
Date modified: May 26, 2016
*/
//Class imports
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
//The main method for a server application to send large prime numbers to a client application through TCP/IP socketing
int main(int argc, char *argv[]){
    int lfd = 0, cfd = 0; //Initialize variables for socket
    struct sockaddr_in servAdd; //Struct for socket
    char sBuffer[1025]; //Output buffer char array
    lfd = socket(AF_INET, SOCK_STREAM, 0); //Create a new unnamed socket and store its file descriptor
    memset(&servAdd, '0', sizeof(servAdd)); //Initialize socket struct
    memset(sBuffer, '0', sizeof(sBuffer)); //Initialize output buffer
    servAdd.sin_family = AF_INET; //Set family/domain
    servAdd.sin_addr.s_addr = htonl(INADDR_ANY); //Set interface for server to listen on
    servAdd.sin_port = htons(5000); //Set port to wait for client response on
    bind(lfd, (struct sockaddr*)&servAdd, sizeof(servAdd)); //Bind the socket details to the created server struct, servers must bind with specific port
    listen(lfd, 12); //Set max number of client connections for server
    while(1){ //Wait for client responses
        cfd = accept(lfd, (struct sockaddr*)NULL, NULL); //Once client is ready, retrieve the client socket file descriptor
        //snprintf(sBuffer, sizeof(sBuffer), "%.24s\r\n", ctime(&ticks));
        write(cfd, sBuffer, strlen(sBuffer)); //Use client descriptor to write back to client
        close(cfd); //Close the current client socket using its file descriptor
        sleep(1);
     } //End while
} //End main