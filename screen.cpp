#include<curses.h>
#include "getChar.hpp"
#include<stdlib.h>
#include <ostream>
#include <unistd.h>
#include <iostream>

#define LINE_WIDTH 80
#define RECEIVE_START_Y 14
#define DIVIDER_Y 13
#define SEND_START_Y 12

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

//format is (Y,X) instead of (X,Y)

int main(void)
{

    for(int i = 0; i < 12; i++){
        for(int j = 0; j < LINE_WIDTH; j++){
            send1[i][j] = '1';
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


     while(c != '\04'){
         c = getchar();
         if(c == '\04'){
             terminate();
         }
         else {
             mvaddch(0, 0, (int) c);
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
        for(int j = 0; j < LINE_WIDTH; j++){
            if (send1[i-1][j] != '~'){
                send1[i-1][j] = send1[i][j];
                mvaddch(i-1, j, send1[i-1][j]);
                refresh();
            }
        }
    }
    char e = ' ';
    for(int i = 0; i < LINE_WIDTH; i++){
        send1[12][i] = e;
        mvaddch(12, i, e);
        refresh();
    }
    sendX = 0;
}