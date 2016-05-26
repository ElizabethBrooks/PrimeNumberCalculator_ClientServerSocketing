/*
Name: Elizabeth Brooks
File: primeCalculator_client
Date Modified: May 26, 2016
*/
//Class imports
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
//The main method for a client application to send large prime numbers to a client application through TCP/IP socketing
int main(int argc, char *argv[]){
    int sfd = 0, count = 0; //Initialize variables for socket conection
    struct sockaddr_in servAdd; //Struct for socket
    char rBuffer[1024]; //Input buffer char array
    if(argc != 2){
        printf("\n Usage: %s <ip of server> \n",argv[0]); //Print ip
        return 1; //Exit program return
    } //End if 
    memset(rBuffer, '0',sizeof(rBuffer)); //Initialize input buffer
    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //Create and verift socket connection to server
        printf("\n Error : Could not create socket \n"); //Print error message if failed
        return 1; //Exit program return
    } //End if
    memset(&servAdd, '0', sizeof(servAdd)); //Initialize socket struct
    servAdd.sin_family = AF_INET; //Set socket family/domain
    servAdd.sin_port = htons(5000); //Set port for socket connection
    if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr)<=0){ //Attempt to set internet pton
        printf("\n inet_pton error occured\n"); //Print error if internet pton failed
        return 1; //Exit program return
    } //End if
    if(connect(sfd, (struct sockaddr *)&servAdd, sizeof(servAdd)) < 0){ //Attempt connection between client and server sockets
       printf("\n Error : Connection Failed \n"); //Print error message if connection failed
       return 1; //Exit program return
    } //End if
    while ((count = read(sfd, rBuffer, sizeof(rBuffer)-1)) > 0){ //Retrieve prime number sent through server/client socket connection
        rBuffer[count] = 0;
        if(fputs(rBuffer, stdout) == EOF){ //Check if buffer is empty
            printf("\n Error : fputs error\n"); //Print error message for input buffer
        } //End if
    } //End while
    if(count < 0){ //Check number sent is valid
        printf("\n Read error \n"); //Print error message for invalid prime number
    } //End if
    return 0; //End of program return
} //End main