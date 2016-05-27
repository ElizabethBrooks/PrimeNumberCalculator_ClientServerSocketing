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
    int sfd=0, num=0, numDiv=0, defaultCheck=0, checkPrime=0, timeout=5; //Initialize variables for socket conection
    struct sockaddr_in servAdd; //Struct for socket
    const char equalsDel[2]="="; //Set delimiter for string tokenization
    char rBuffer[1044]; //Input buffer char array
    FILE *userFile=0; //Initialize input file object
    size_t lineSize = 144; //Set file line buffer size
    char *line = malloc(lineSize * sizeof(char)); //Allocate memory for line buffer
    memset(rBuffer, '0', sizeof(rBuffer)); //Initialize input buffer
    memset(&servAdd, '0', sizeof(servAdd)); //Initialize socket struct
    if(argc > 2){ //Verify input parameters
        printf("\nError: %s file usage\n",argv[0]); //Print filename
        return 1; //Exit program return
    } //End if
    userFile=fopen(argv[1], "rt"); //Open input file from command line
    if(userFile){ //Verify the file exists
        if(getline(&line, &lineSize, userFile)){ //Retrieve first line with ip
            servAdd.sin_addr.s_addr=inet_addr(line); //Set ip
        }else{ //File error
            defaultCheck=1;
        } //End if, else
        if(getline(&line, &lineSize, userFile)){ //Retrieve second line with port
            servAdd.sin_port=htons(atoi(line)); //Set port
        }else{ //File error
            defaultCheck=1;
        } //End if, else
        if(getline(&line, &lineSize, userFile)){ //Retrieve third line with timeout
            timeout=atoi(line); //Convert string token to int and set timeout
        }else{ //File error
            defaultCheck=1;
        } //End if, else
        if(defaultCheck==1){ //Check if file error occured
            printf("\nError: File content invalid... using default ip, port, and timeout values.\n"); //Print error message
            servAdd.sin_port=htons(4488); //Set default port
            servAdd.sin_addr.s_addr=inet_addr("192.168.1.100"); //Set default ip
        }
    }else{ //Return error if file does not exist
        printf("\nError: File not found... using default ip, port, and timeout values.\n"); //Print error message
        servAdd.sin_port=htons(4488); //Set default port
        servAdd.sin_addr.s_addr=inet_addr("192.168.1.100"); //Set default ip
    }//End if, else
    fclose(userFile); //Close input file 
    servAdd.sin_family=AF_INET; //Set socket family/domain  
    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //Create and verift socket connection to server
        printf("\nError: socket creation failure\n"); //Print error message if failed
        return -1; //Exit program with error
    } //End if
    while(timeout>0){ //Attempt to connect to server for the amount of time specified
        //printf("connect");
        if(connect(sfd, (struct sockaddr *)&servAdd, sizeof(servAdd)) < 0){ //Attempt connection between client and server sockets
           timeout--; //Decrement connection timer
           sleep(1); //Wait for server
        }//End if
        if(timeout<=0){ 
            printf("\nError: connection failure\n"); //Print error message if connection failed
            return -1; //Exit program, unable to connect in given time
        } //End if
    } //End while
    if((num=read(sfd, rBuffer, sizeof(rBuffer)-1)) < 0){ //Retrieve prime number sent through server/client socket connection
        printf("\nError : socket read failure\n"); //Print error message for read failure
        return -1; //Exit program with error
    } //End if
    bzero(rBuffer, 1044); //Clear output buffer
    if((numDiv=read(sfd, rBuffer, sizeof(rBuffer)-1)) < 0){ //Retrieve divisor
        printf("\nError : socket read failure\n"); //Print error message for read failure
        return -1; //Exit program with error
    } //End if
    if(num%numDiv==0){ //Determine if input number is prime
        checkPrime=1; //Not prime
        if(send(sfd, &checkPrime, sizeof(int), 0) < 0){ //Attempt to send to server check value
            printf("Error: send failure"); //Print error message for send failure
            return -1; //Exit program with error
        } //End if
    }else{
        checkPrime=0; //Keep checking
        if(send(sfd, &checkPrime, sizeof(int), 0) < 0){ //Attempt to send to server check value
            printf("Error: send failure"); //Print error message for send failure
            return -1; //Exit program with error
        } //End if
    } //End else, if
    if((fputs(rBuffer, stdout)) == EOF){ //Check if buffer is empty
        printf("\nError : results display failure\n"); //Print error message for output buffer
        return -1; //Exit program with error
    } //End if
    if(num < 0){ //Check number sent is valid
        printf("\nError: read failure\n"); //Print error message for invalid prime number
        return -1; //Exit program with error
    } //End if
    return 0; //End of program return
} //End main