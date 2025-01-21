CC=g++ 
CFLAGS=-c -w
# -Wall

all: Serveur main.o VSCOM.o SNPortSerie.o ServeurTCP.o 

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -std=c++11

VSCOM.o: VSCOM.cpp VSCOM.h
	$(CC) $(CFLAGS) VSCOM.cpp

SNPortSerie.o: SNPortSerie.h SNPortSerie.cpp
	$(CC) $(CFLAGS) SNPortSerie.cpp

ServeurTCP.o: ServeurTCP.h ServeurTCP.cpp
	$(CC) $(CFLAGS) ServeurTCP.cpp

Serveur: main.o VSCOM.o SNPortSerie.o ServeurTCP.o 
	$(CC) main.o VSCOM.o SNPortSerie.o ServeurTCP.o -lpthread -o Serveur 

clean :
	rm -f *.o Serveur

