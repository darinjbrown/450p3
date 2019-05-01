#include<curses.h>
#include "getChar.hpp"

void startup( void );
void terminate( void );

int main(void)
{
     int c;

     startup();
     move(0, 0);  // move the curser to the top- and left-most block.
     addstr("0123456789012345678901234567890123456789");
     addstr("0123456789012345678901234567890123456789");
     move(1, 0);  // move the curser to the second row, first column.
     addch('*');  // put a character at (1, 0)
     move(1, 20);
     addstr( "This string starts on column 21.");
     mvaddch(1, 79, '*'); // same as: move(1, 79); addch('*');
     mvaddch(22, 0, '*');
     move(12, 30);
     addstr( "--------------------------------------------------------------");
     mvaddch(22, 79, '*');
     refresh();  // this function call forces the screen to be updated. 
     // the following function moves the curser to (4, 5) and then
     // writes a string.  It is equivalent to the following two stmts:
     // move(4, 5);
     // addstr("Type in a non-blank character, after it is echoed ");
     mvaddstr(4, 5, "Type in a couple of non-blank characters ");
     addstr("and then wait and watch!"); 
     refresh();
     for(int i = 13; i < 25; i++) {
	  mvaddch( i, i, get_char() );
	  refresh();
     }	  
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
