CC=g++ 
CFLAGS=-c -w

all: Json main.o JSONFile.o 

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -std=c++11

JSONFile.o: JSONFile.h JSONFile.cpp
	$(CC) $(CFLAGS) JSONFile.cpp

Json: main.o  JSONFile.o
	$(CC) main.o  JSONFile.o -o  Json

clean :
	rm -f *.o Json