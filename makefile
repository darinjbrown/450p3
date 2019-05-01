screen.x:  screen.o getChar.o
	g++ -ggdb -o screen.x screen.o getChar.o -lcurses -ltermcap

screen.o: screen.cpp
	g++ -c -ggdb screen.cpp -o screen.o

getChar.o: getChar.cpp 
	gcc -c -ggdb getChar.cpp -o getChar.o

clean:
	/bin/rm -f screen.x screen.o getChar.o *~
