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
std::string new1 = "newline (backslash n) read";


//format is (Y,X) instead of (X,Y)

int main(void)
{
    char c;
    for(int i = 0; i < 12; i++){
        for(int j = 0; j < LINE_WIDTH; j++){
            send1[i][j] = '~';
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


     for(int i = 0; i < 100; i++){
         c = getchar();
         if (c == '\n'){
             addch('!');
             for (int i = 1; i < SEND_START_Y; i++){
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
             d = c;
             mvaddch(y, sendX, d);
             send1[y][sendX] = d;
             sendX++;
             move(y, sendX);

         }

         refresh();
         if(sendX >= LINE_WIDTH) {
             sendX = 0;
             move(y, sendX);
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


