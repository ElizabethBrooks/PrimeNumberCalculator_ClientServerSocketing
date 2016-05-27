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
#include <limits.h>
#include <pthread.h>
#include <math.h>
int randomNum, divisor=2; //Initialize global varibles for determing if random number is prime
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER; //Allow for mutual exclusion to handle critical sections
//Method for sending random numbers to clients using threads
void *send_random(void *lfdInput){
    int parameter, checkPrime=0; //Initialize parameter for lfd input
    memcpy(&parameter,(int *)lfdInput,sizeof(int)); //Store input lfd in parameter
    int cfd=0; //Initialize variable for sending numbers to clients
    char sBuffer[1044], rBuffer[1044]; //Output buffer char array
    memset(sBuffer, '0', sizeof(sBuffer)); //Initialize output buffer
    memset(rBuffer, '0', sizeof(rBuffer)); //Initialize output buffer
    while(divisor!=sqrt(randomNum)){ //Wait for client responses
        cfd=accept(parameter, (struct sockaddr*)NULL, NULL); //Once client is ready, retrieve the client socket file descriptor
        snprintf(sBuffer, sizeof(sBuffer), "%d", randomNum); //Send random number to output buffer
        write(cfd, sBuffer, strlen(sBuffer)); //Use client descriptor to write back to client the random number
        snprintf(sBuffer, sizeof(sBuffer), "%d", divisor); //Send random number to output buffer
        write(cfd, sBuffer, strlen(sBuffer)); //Use client descriptor to write back to client the divisor
        pthread_mutex_lock(&count_mutex);//Mutually exclude multiple threads from divisor global counter
        divisor++; //Increment the divisor global counter
        pthread_mutex_unlock(&count_mutex);//Unlock mutually excluded threads from divisor global counter
        checkPrime=recv(cfd, rBuffer, strlen(rBuffer), 0); //Recieve output from client
        if(checkPrime==1){ //Number is not prime
            printf("%d is not a prime number!", randomNum); //Print error message to the screen
            return 0; //Exit method return
        } //End if
    } //End while
    close(cfd); //Close the current client socket using its file descriptor
    sleep(1); //Pause client process
} //End send_random
//The main method for a server application to send large prime numbers to a client application through TCP/IP socketing
int main(int argc, char *argv[]){
    randomNum=rand()%(INT_MAX-1)+(INT_MAX-1000000); //Generate random, large number
    randomNum=randomNum/(-10); //Reduce size of number to avoid overflow
    int lfd=0, threadNum=0, i=0; //Initialize variables for socket
    struct sockaddr_in servAdd; //Struct for socket
    lfd=socket(AF_INET, SOCK_STREAM, 0); //Create a new unnamed socket and store its file descriptor
    memset(&servAdd, '0', sizeof(servAdd)); //Initialize socket struct
    servAdd.sin_addr.s_addr=inet_addr("192.168.1.100"); //Set interface for server to listen on
    servAdd.sin_port=htons(4488); //Set port to wait for client response on
    servAdd.sin_family=AF_INET; //Set family/domain
    bind(lfd, (struct sockaddr*)&servAdd, sizeof(servAdd)); //Bind the socket details to the created server struct, servers must bind with specific port
    listen(lfd, 5); //Set max number of client connections for server, 5 is recommended max for most systems
    pthread_t threadPool[5]; //Thread pool structure
    for(threadNum=0; threadNum < 5; threadNum++){ //Loop and create threads
        fflush(stdout); //Flush output buffer
        int Error = pthread_create(&threadPool[threadNum],NULL,send_random,(void *)&lfd); //Create threads for searching neighborhoods
        if (Error) { //Catch errors in creating threads
            printf("Error: creating threads... program exited."); //Print error message to the screen
            return -1; //Exit program with error
      } //End if
    } //End for
    for(i=0; i < 5; i++){ //Loop through created client threads
        pthread_join(threadPool[i],(void **)0);//Join client threads
    } //End for
    printf("%d is a prime number!", randomNum); //Print error message to the screen
    return 0; //End of program return
} //End main