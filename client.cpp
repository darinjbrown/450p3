//
//
//

#include<curses.h>
#include "getChar.hpp"
#include <iostream>
#include <ostream>
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

#include <thread>
#include <pthread.h>

#define PORT_NUMBER 8016
#define PORT_NUMBER2 8017
#define MAXLINE 80
#define LISTENQ 5
#define LINE_WIDTH 80
#define RECEIVE_START_Y 14
#define DIVIDER_Y 13
#define SEND_START_Y 12
// The client, as an argument, takes the IP of a server and attempts

// to connect to it.

// To run this program, a server program must already have been
// launched.  To compile, do:

// g++ -std=c++17 client.cpp -o client.x
// To run, do:

// ./client.x IP-of-the server.

// If the server is running on the local machine, you can do:

// ./client.x 127.0.0.1

void startup( void );
void terminate( void );

char dividerChar = '_';
char d;
int receiveX = 0;
int sendX = 0;
int y = SEND_START_Y;
int receiveY = RECEIVE_START_Y;
char send1[12][LINE_WIDTH];
char receive1[12][LINE_WIDTH];
void scrollOneLine();
int initOut();
int initIn();
void typeAndWrite(int outgoingSocketfd);
void readAndDisplay(int incomingSocketfd);

int sockfd, n, listenfd, connfd;
char recvline[MAXLINE + 1];
struct sockaddr_in servaddr, cliaddr, servaddr2;
socklen_t len;
char buff[MAXLINE];
char c, w, r;
void writeThreadFunction(int socketFD); // this thread will write to curses and the connection to server through passed file descriptor

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

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "inet_pton error for %s\n", argv[1]);
        exit(3);
    }

//************
    int outgoingSocketfd = initOut();
    int incomingSocketfd = initIn();

    std::thread write_thread(writeThreadFunction, outgoingSocketfd);

    write_thread.join();

    close(outgoingSocketfd);

  }



    //initiate curses and write to both the socket and curses
void writeThreadFunction(int outgoingSocketfd){
    for(int i = 0; i < 12; i++){
        for(int j = 0; j <= LINE_WIDTH; j++){
            send1[i][j] = ' ';
        }
    }
    startup();
    move(DIVIDER_Y, 0);  // move the cursor to sender's start
    for (int i = 0; i < LINE_WIDTH; i++){
        addch(dividerChar);
        refresh();
    }

    refresh();
    move(SEND_START_Y, sendX);


    while(c != '\x04'){
        c = getchar();
        if(c == '\x04'){
            terminate();
        }
        else {
            if (c == '\x04' || c == '\x1a'){
                terminate();
                close(sockfd);
                exit(0);
            }
            //std::cout << "about to send char\n"; // for debugging
            if (send(outgoingSocketfd , (char *)&c, sizeof(c), 0 ) < 0){
                fprintf( stderr, "Write failed. %s\n", strerror(errno));
                exit(1);
            }

            if (c == '\n' || c == '\r') {
                move(0, 0);
                addstr("Got a new line char.");
                addch('!');
                scrollOneLine();
            } else {
                mvaddch(y, sendX, c);
                send1[y][sendX] = c;
                sendX++;
                move(y, sendX);
            }

            refresh();
            if (sendX >= LINE_WIDTH) {
                scrollOneLine();
            }
        }
    }

    sleep(1);
    terminate();
    close(outgoingSocketfd);
}

int initOut(){
// Create a socket end-point for communication.  We use the

    // Attempt to connect to the server.
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr, "connect error: %s\n", strerror(errno));
        exit(4);
    }
    return sockfd;
}

int initIn(){
//****************************************************************
    // create listening server for 'server' to connect to as client

    bzero(&servaddr2, sizeof(servaddr2));
    servaddr2.sin_family      = AF_INET;  // Communicate using the Internet domain (AF_INET)
    servaddr2.sin_addr.s_addr = htonl(INADDR_ANY);   // Who should we accept connections from?
    servaddr2.sin_port        = htons(PORT_NUMBER2);  // Which port should the server listen on?


    std::cout << "just tried to create servaddr2 struct\n";

    // Create an end-point for IPv4 Internet Protocol
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "2nd socket failed.  %s\n", strerror(errno));
        exit(1);

    }

    std::cout << "just tried to create listenfd socket\n";

    // Bind the server end-point using the specifications stored in "serveraddr2"
    if (bind(listenfd, (struct sockaddr *) &servaddr2, sizeof(servaddr2)) < 0) {
        fprintf(stderr, "Bind of 2nd socket failed.  %s\n", strerror(errno));
        exit(1);
    }

    std::cout <<"just bound listenfd\n";

    // Listen on the in-comming connections; pile up at most LISTENQ number of connections.
    if (listen(listenfd, LISTENQ) < 0) {
        fprintf(stderr, "Listen failed.  %s\n", strerror(errno));
        exit(1);
    }

    std::cout << "just executed listen on listenfd\n";

    for ( ; ; ) {
        len = sizeof(cliaddr);
        // establish a connection with an incoming client.
        std::cout << "about to execute accept on listenfd\n";
        if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len)) < 0) {
            fprintf(stderr, "Accept failed.  %s\n", strerror(errno));
            exit(1);
        }

        std::cout << "just executed accept on listenfd\n";

        printf("connection from %s, port %d\n",
               inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
               ntohs(cliaddr.sin_port));
        return connfd;
    }

        //***************************************************************
}
void startup( void )
{
    initscr();	    /* activate curses                                            */
    curs_set(0);   /* prevent the cursor to be displayed                         */
    clear();	    /* clear the screen that curses provides                      */
    noecho();	    /* prevent the input chars to be echoed to the screen         */
    cbreak();	    /* change the stty so that characters are delivered to the
		       program as they are typed--no need to hit the return key!  */
}

void terminate( void )
{
    mvcur( 0, COLS - 1, LINES - 1, 0 );
    clear();
    refresh();
    endwin();
}

void scrollOneLine(){
    for (int i = 1; i <= SEND_START_Y; i++){
        for(int j = 0; j <= LINE_WIDTH; j++){
            if (send1[i-1][j] != '~'){
                send1[i-1][j] = send1[i][j];
                mvaddch(i-1, j, send1[i-1][j]);
                refresh();
            }
        }
    }
    char e = ' ';
    for(int i = 0; i <= LINE_WIDTH; i++){
        send1[12][i] = e;
        mvaddch(12, i, e);
        refresh();
    }
    sendX = 0;
}