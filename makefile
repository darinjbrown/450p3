server.x: server.o getChar.o
	g++ -std=c++17 -o server.x

server.o: server.cpp getChar.o
	g++ -c -ggdb server.cpp -o server.o

client.x: client.o getChar.o
	g++ -std=c++17 -o client.x

client.o: client.cpp getChar.o
	g++ -c -ggdb client.cpp -o client.o

screen.x:  screen.o getChar.o
	g++ -ggdb -o screen.x screen.o getChar.o -lcurses -ltermcap

screen.o: screen.cpp
	g++ -c -ggdb screen.cpp -o screen.o

getChar.o: getChar.cpp 
	gcc -c -ggdb getChar.cpp -o getChar.o

clean:
	/bin/rm -f screen.x screen.o getChar.o *~
