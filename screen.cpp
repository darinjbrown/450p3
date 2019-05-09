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
int receiveX = 0;
int sendX = 0;
int y = SEND_START_Y;
int receiveY = RECEIVE_START_Y;
char send1[12][LINE_WIDTH];
char receive1[12][LINE_WIDTH];


//format is (Y,X) instead of (X,Y)

int main(void)
{
     int c;
    for(int i = 0; i < 12; i++){
        for(int j = 0; j < LINE_WIDTH; j++){
            send1[i][j] = '~';
        }
    }
     startup();
     move(DIVIDER_Y, 0);  // move the curser to sender's start
     for (int i = 0; i < LINE_WIDTH; i++){
         addch(dividerChar);
         refresh();
     }
     move(0, SEND_START_Y);

     refresh();  // this function call forces the screen to be updated. 
     // the following function moves the curser to (4, 5) and then
     // writes a string.  It is equivalent to the following two stmts:
     // move(4, 5);
     // addstr("Type in a non-blank character, after it is echoed ");
     mvaddstr(4, 5, "Type in a couple of non-blank characters ");
     addstr("and then wait and watch!"); 
     refresh();
     move(SEND_START_Y, sendX);
     for(int i = 0; i < 100; i++){
         c = getchar();
         if (c == '\n'){
             for (int i = 0; i < SEND_START_Y; i++){
                 for(int j = 0; j < LINE_WIDTH; j++){
                     if (send1[j][i] != '~'){
                         send1[j-1][i] = send1[j][i];
                         mvaddch(j-1, i, send1[j-1][i]);
                         refresh();
                     }
                 }
             }
             sendX = 0;

         }else {
             mvaddch(y, sendX, c);
             send1[y][sendX] = c;
             sendX++;

         }

         refresh();
         if(sendX > LINE_WIDTH) sendX = 0;
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


