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
#include <math.h>
//The main method for a client application to send large prime numbers to a client application through TCP/IP socketing
int main(int argc, char *argv[]){
    int sfd=0, num=0, numDiv=0, timeout=5; //Initialize variables for socket conection
    struct sockaddr_in servAdd; //Struct for socket
    const char equalsDel[2]="="; //Set delimiter for string tokenization
    char rBuffer[1044], fileString[144], *token="ini", line[144]; //Input buffer char array
    FILE *userFile=0; //Initialize input file object
    memset(rBuffer, '0', sizeof(rBuffer)); //Initialize input buffer
    memset(&servAdd, '0', sizeof(servAdd)); //Initialize socket struct
    if(argc != 2){ //Verify input parameters
        printf("\nUsage: %s filename\n",argv[0]); //Print filename
        return 1; //Exit program return
    } //End if
    userFile=fopen(argv[1], "rt"); //Open input file from command line
    if(userFile){ //Verify the file exists
        while(fgets(line, 144, userFile) != NULL){ //Parse input file by line, up to 144 characters
            token=strtok(line, equalsDel); //Retrieve first token
            while(token != NULL){ //Tokenize file string for input parameters
                if(strcmp(token,"ip")){ //Check if ip
                    token=strtok(NULL, equalsDel); //Retrieve next token
                    servAdd.sin_addr.s_addr=inet_addr(token); //Set ip
                }else if(strcmp(token,"port")){ //Check if port
                    token=strtok(NULL, equalsDel); //Retrieve next token
                    servAdd.sin_port=htons(atoi(token)); //Set port
                }else if(strcmp(token,"timeout")){ //Check if timeout
                    token=strtok(NULL, equalsDel); //Retrieve next token
                    timeout=atoi(token); //Convert string token to int
                }else{ //Invalid input file string token
                    printf("\nError: invalid input file... using default ip, port, and timeout values.\n"); //Print error message
                    servAdd.sin_port=htons(4444); //Set default port
                    servAdd.sin_addr.s_addr=inet_addr("192.168.1.100"); //Set default ip
                } //End if, else if
            } //End inner while
        } //End outter while
    }else{ //Return error if file does not exist
        printf("File not found... using default ip, port, and timeout values.\n"); //Print error message
        servAdd.sin_port=htons(4444); //Set default port
        servAdd.sin_addr.s_addr=inet_addr("192.168.1.100"); //Set default ip
    }//End if, else
    fclose(userFile); //Close input file 
    servAdd.sin_family=AF_INET; //Set socket family/domain   
    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //Create and verift socket connection to server
        printf("\nError: socket creation failure\n"); //Print error message if failed
        return -1; //Exit program with error
    } //End if
    while(timeout>0){ //Attempt to connect to server for the amount of time specified
        if(connect(sfd, (struct sockaddr *)&servAdd, sizeof(servAdd)) < 0){ //Attempt connection between client and server sockets
           timeout--; //Decrement connection timer
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
        printf("%d is not a prime number!\n", num); //Print that the number is not prime
        return 0; //End program return
    } //End if
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