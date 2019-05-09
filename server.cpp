//
// Created by Darin on 5/1/2019.
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

#include<time.h>

#include<curses.h>
#include "getChar.hpp"


#define PORT_NUMBER 8016
#define PORT_NUMBER2 8017
#define MAXLINE 200
#define LISTENQ 5

// This server program creates a TCP socket and listen on port "PORT_NUMBER".

// To compile, do:
// gcc server.c -o server.x
// To run, do:
// ./server.x

// A server must be launched before clients can talk to it!

int initIn();
int initOut();
int	listenfd, connfd, sock2fd;
socklen_t   len;
struct sockaddr_in  servaddr, cliaddr, servaddr2;
char	buff[MAXLINE];
char w, r;
int main(int argc, char **argv)
{

    time_t	ticks;

    int incomingFD = initIn();
    int outgoingFD = initOut();

    while(r != '~'){

        ssize_t read(incomingFD, (char *)&r, 1);

        std::cout << r << std::endl;

    }
//    // Create an end-point for IPv4 Internet Protocol
//    if( ( listenfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ) {
//        fprintf( stderr, "socket failed.  %s\n", strerror( errno ) );
//        exit( 1 );
//
//    }
//
//
//    bzero(&servaddr, sizeof(servaddr));
//    servaddr.sin_family      = AF_INET;  // Communicate using the Internet domain (AF_INET)
//    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   // Who should we accept connections from?
//    servaddr.sin_port        = htons(PORT_NUMBER);  // Which port should the server listen on?
//
//
//    // Bind the server end-point using the specifications stored in "serveraddr"
//    if( bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
//        fprintf( stderr, "Bind failed.  %s\n", strerror( errno ) );
//        exit( 1 );
//    }
//
//    // Listen on the in-comming connections; pile up at most LISTENQ number of connections.
//    if( listen(listenfd, LISTENQ) < 0 ) {
//        fprintf( stderr, "Listen failed.  %s\n", strerror( errno ) );
//        exit( 1 );
//    }
//
//    for ( ; ; ) {
//        len = sizeof(cliaddr);
//        // establish a connection with an incoming client.
//        if( ( connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len) ) < 0 ) {
//            fprintf( stderr, "Accept failed.  %s\n", strerror( errno ) );
//            exit( 1 );
//        }
//        printf("connection from %s, port %d\n",
//               inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
//               ntohs(cliaddr.sin_port));

//        //****************************************************************************
//        // ATTEMPT TO CONNECT (client.cpp as server, this program as client)
//
//
//        bzero(&servaddr2, sizeof(servaddr2));
//        servaddr2.sin_family      = AF_INET;  // Communicate using the Internet domain (AF_INET)
//        servaddr2.sin_port        = htons(PORT_NUMBER2);  // Which port should the server listen on?
//
////        if (inet_pton(AF_INET, "127.0.0.1", &servaddr2.sin_addr) <= 0) {
////            fprintf(stderr, "inet_pton error for %s\n", argv[1]);
////            exit(3);
////        }
//
//        sleep(5); // give client time to start listening before attempting connection
//        sock2fd = socket(AF_INET, SOCK_STREAM, 0);
//        std::cout<<"just attempted to create sock2fd\n";
//
//        if (connect(sock2fd, (struct sockaddr *) &servaddr2, sizeof(servaddr2)) < 0) {
//            fprintf(stderr, "sock2fd connect error: %s\n", strerror(errno));
//            exit(4);
//        }
//        std::cout << "just tried to connect through sock2fd\n";
//
//
//        //*******************************************************************************

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        if( write(incomingFD, buff, strlen(buff)) < 0 ) {
            fprintf( stderr, "Write failed.  %s\n", strerror( errno ) );
            exit( 1 );
        }
        // finished talking to this client.  Close the connection.
//        std::string closer;
//        std::cin >> closer;
//        if (closer == "q") close(connfd);
}



int initIn(){
    // Create an end-point for IPv4 Internet Protocol
    if( ( listenfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ) {
        fprintf( stderr, "socket failed.  %s\n", strerror( errno ) );
        exit( 1 );

    }


    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;  // Communicate using the Internet domain (AF_INET)
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   // Who should we accept connections from?
    servaddr.sin_port        = htons(PORT_NUMBER);  // Which port should the server listen on?


    // Bind the server end-point using the specifications stored in "serveraddr"
    if( bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
        fprintf( stderr, "Bind failed.  %s\n", strerror( errno ) );
        exit( 1 );
    }

    // Listen on the in-comming connections; pile up at most LISTENQ number of connections.
    if( listen(listenfd, LISTENQ) < 0 ) {
        fprintf( stderr, "Listen failed.  %s\n", strerror( errno ) );
        exit( 1 );
    }

    for ( ; ; ) {
        len = sizeof(cliaddr);
        // establish a connection with an incoming client.
        if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len)) < 0) {
            fprintf(stderr, "Accept failed.  %s\n", strerror(errno));
            exit(1);
        } else {
            printf("connection from %s, port %d\n",
                   inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
                   ntohs(cliaddr.sin_port));
            return connfd;
        }
    }

}

int initOut(){
    //****************************************************************************
    // ATTEMPT TO CONNECT (client.cpp as server, this program as client)


    bzero(&servaddr2, sizeof(servaddr2));
    servaddr2.sin_family      = AF_INET;  // Communicate using the Internet domain (AF_INET)
    servaddr2.sin_port        = htons(PORT_NUMBER2);  // Which port should the server listen on?

//        if (inet_pton(AF_INET, "127.0.0.1", &servaddr2.sin_addr) <= 0) {
//            fprintf(stderr, "inet_pton error for %s\n", argv[1]);
//            exit(3);
//        }

    sleep(5); // give client time to start listening before attempting connection
    sock2fd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout<<"just attempted to create sock2fd\n";

    if (connect(sock2fd, (struct sockaddr *) &servaddr2, sizeof(servaddr2)) < 0) {
        fprintf(stderr, "sock2fd connect error: %s\n", strerror(errno));
        exit(4);
    }
    std::cout << "just tried to connect through sock2fd\n";


    //*******************************************************************************

}