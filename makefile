
client.x:  client.o getChar.o
	g++ -ggdb -o client.x client.o getChar.o -lcurses -ltermcap

client.o: client.cpp getChar.o
	g++ -c -ggdb screen.cpp -o screen.o

server.x:  server.o getChar.o
	g++ -ggdb -o server.x server.o getChar.o -lcurses -ltermcap

server.o: server.cpp getChar.o
	g++ -c -ggdb screen.cpp -o screen.o

screen.x:  screen.o getChar.o
	g++ -ggdb -o screen.x screen.o getChar.o -lcurses -ltermcap

screen.o: screen.cpp
	g++ -c -ggdb screen.cpp -o screen.o

getChar.o: getChar.cpp 
	gcc -c -ggdb getChar.cpp -o getChar.o

clean:
	/bin/rm *.o *.x