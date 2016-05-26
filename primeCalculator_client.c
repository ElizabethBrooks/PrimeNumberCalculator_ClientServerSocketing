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
//Method for efficiently determining if an input number is prime
int primeCalculator(int numInput){
   int i; //Initialize counter
   for(i=2; i < sqrt(numInput); i++){ //Check values until the sqrt of number
      if(numInput%i == 0){ //Check if number is divisible by i
         return 0; //Number is not prime
      } //End if
   } //End for
   return 1; //Number is prime
} //End primeCalculator
//The main method for a client application to send large prime numbers to a client application through TCP/IP socketing
int main(int argc, char *argv[]){
    int sfd=0, num=0, timeout=0; //Initialize variables for socket conection
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
            sscanf(line, "%s", &fileString); //Retrieve current file line
            token=strtok(fileString, equalsDel); //Retrieve first token
            while(token != NULL){ //Tokenize file string for input parameters
                if(strcmp(token,"ip")){ //Check if ip
                    token=strtok(NULL, equalsDel); //Retrieve next token
                    localaddr.sin_addr.s_addr=inet_addr(token); //Set ip
                }else if(strcmp(token,"port")){ //Check if port
                    token=strtok(NULL, equalsDel); //Retrieve next token
                    servAdd.sin_port=htons(token); //Set port
                }else if(strcmp(token,"timeout")){ //Check if timeout
                    token=strtok(NULL, equalsDel); //Retrieve next token
                    sscanf(token, "%d", &timeout); //Convert string token to int
                }else{ //Invalid input file string token
                    printf("\nError: invalid input file... using default ip, port, and timeout values.\n"); //Print error message
                    servAdd.sin_port=htons(4444); //Set default port
                    servAdd.sin_addr.s_addr=inet_addr("192.168.1.100"); //Set default ip
                } //End if, else if
            } //End while
        } //End while
    }else{ //Return error if file does not exist
        printf("File not found... using default ip, port, and timeout values.\n"); //Print error message
        servAdd.sin_port=htons(4444); //Set default port
        servAdd.sin_addr.s_addr=inet_addr("192.168.1.100"); //Set default ip
    }//End else
    fclose(userFile); //Close input file 
    servAdd.sin_family=AF_INET; //Set socket family/domain   
    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //Create and verift socket connection to server
        printf("\nError: socket creation failure\n"); //Print error message if failed
        return -1; //Exit program with error
    } //End if
    if(connect(sfd, (struct sockaddr *)&servAdd, sizeof(servAdd)) < 0){ //Attempt connection between client and server sockets
       printf("\nError: connection failure\n"); //Print error message if connection failed
       return -1; //Exit program with error
    } //End if
    while((num=read(sfd, rBuffer, sizeof(rBuffer)-1)) > 0){ //Retrieve prime number sent through server/client socket connection
        rBuffer[num]=0; //Clear output buffer
        if(primeCalculator(num) == 1){ //Determine if input number is prime
            printf("%d is a prime number!\n", num); //Print that the number is prime
        }else{ //Input number is not prime
            printf("%d is not a prime number!\n", num); //Print that the number is not prime
        } //End if, else
        if(fputs(rBuffer, stdout) == EOF){ //Display result of calcuation
            printf("\nError : results display failure\n"); //Print error message for output buffer
        } //End if
    } //End while
    if(num < 0){ //Check number sent is valid
        printf("\nError: read failure\n"); //Print error message for invalid prime number
    } //End if
    return 0; //End of program return
} //End main