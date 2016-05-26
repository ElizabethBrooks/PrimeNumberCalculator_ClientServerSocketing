Name: Elizabeth Brooks
Date Modified: May 26, 2016
File: CS470 Lab4

#MatrixNeighborhoodSearch_MultiThreading
Lab 4: "Write a client and a server program in C/C++ using TCP/IP. The server program generates
randomly a rather huge number (close to INT_MAX, see for details limits.h), and the client(s)
should decide if the number is prime or not. The checking mechanism should be as follows:
1) The server is sending at a time the number x and a number y ={2…sqrt(x) } to each client
2) The client should check if x is divisible by y –if so the number x is not prime (send to server)
3) Otherwise the server should continue to send new pairs to each client until y == sqrt(x)
or x is divisible by y.
Each client should receive equal number of divisions to perform."
- http://www.cwu.edu/~szilardv/images/uploadedImgs/Operating_Systems_Lab_4.pdf

##To compile: (Using library linkers and input file path)
$ gcc -o testServer primeCalculator_server.c -lpthread -lm
$ gcc -o testClient primeCalculator_client.c
$ ./testServer
$ ./testClient ini.txt