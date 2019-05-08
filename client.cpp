//
//
//
#include <iostream>
#include <cstdlib>

#include<sys/types.h>
#include<sys/socket.h>	/* basic socket definitions */
#include<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include<arpa/inet.h>

#include<sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

#define PORT_NUMBER 8016
#define PORT_NUMBER2 8017
#define MAXLINE 80
#define LISTENQ 5
// The client, as an argument, takes the IP of a server and attempts

// to connect to it.

// To run this program, a server program must already have been
// launched.  To compile, do:

// g++ -std=c++17 client.cpp -o client.x
// To run, do:

// ./client.x IP-of-the server.

// If the server is running on the local machine, you can do:

// ./client.x 127.0.0.1


int sockfd, n, listenfd, connfd;
char recvline[MAXLINE + 1];
struct sockaddr_in servaddr, cliaddr, servaddr2;
socklen_t len;
char buff[MAXLINE];
char c;

int main(int argc, char **argv) {


    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPaddress>\n", argv[0]);
        exit(1);
    }

// Create a socket end-point for communication.  We use the
    // IPv4 format and a stream socket.
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Socket error.  %s\n", strerror(errno));
        exit(2);
    }

    // build a profile for the server to whom we are going to
    // communicate.
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT_NUMBER);

    bzero(&servaddr2, sizeof(servaddr2));
    servaddr2 = servaddr;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   // Who should we accept connections from?
    servaddr2.sin_port = htons(PORT_NUMBER2);


    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "inet_pton error for %s\n", argv[1]);
        exit(3);
    }

    // Attempt to connect to the server.
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr, "connect error: %s\n", strerror(errno));
        exit(4);
    }
    //****************************************************************
    // create listening server for 'server' to connect to as client

    // Create an end-point for IPv4 Internet Protocol
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "2nd socket failed.  %s\n", strerror(errno));
        exit(1);

    }

    // Bind the server end-point using the specifications stored in "serveraddr2"
    if (bind(listenfd, (struct sockaddr *) &servaddr2, sizeof(servaddr2)) < 0) {
        fprintf(stderr, "Bind of 2nd socket failed.  %s\n", strerror(errno));
        exit(1);
    }

    // Listen on the in-comming connections; pile up at most LISTENQ number of connections.
    if (listen(listenfd, LISTENQ) < 0) {
        fprintf(stderr, "Listen failed.  %s\n", strerror(errno));
        exit(1);
    }

    for (;;) {
        len = sizeof(cliaddr);
        // establish a connection with an incoming client.
        if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len)) < 0) {
            fprintf(stderr, "Accept failed.  %s\n", strerror(errno));
            exit(1);
        }
        printf("connection from %s, port %d\n",
               inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
               ntohs(cliaddr.sin_port));

        //***************************************************************

        // talk to the server. (Application-level protocol)
        while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
            recvline[n] = 0;    /* null terminate */
            if (fputs(recvline, stdout) == EOF) {
                fprintf(stderr, "fputs error: %s", strerror(errno));
                exit(5);
            }
        }
        if (n < 0) {
            fprintf(stderr, "read error: %s", strerror(errno));
            exit(6);
        }
        if (c == '~')close(sockfd);
        exit(0);
    }
}

int initIn(){

}

//int initOut(){
//    // Create a socket end-point for communication.  We use the
//    // IPv4 format and a stream socket.
//    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//        fprintf( stderr, "Socket error.  %s\n", strerror( errno ) );
//        exit( 2 );
//    }
//
//    // build a profile for the server to whom we are going to
//    // communicate.
//    bzero(&servaddr, sizeof(servaddr));
//    servaddr.sin_family = AF_INET;
//    servaddr.sin_port   = htons(PORT_NUMBER);
//    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
//        fprintf( stderr, "inet_pton error for %s\n", argv[1] );
//        exit( 3 );
//    }
//
//    // Attempt to connect to the server.
//    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
//        fprintf( stderr, "connect error: %s\n", strerror( errno ) );
//        exit( 4 );
//    }
//}