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

SNIRconversions.o: SNIRconversions.h SNIRconversions.cpp
	$(CC) $(CFLAGS) SNIRconversions.cpp

MessageCAN.o: MessageCAN.h MessageCAN.cpp
	$(CC) $(CFLAGS) MessageCAN.cpp

JSONFile.o: JSONFile.h JSONFile.cpp
	$(CC) $(CFLAGS) JSONFile.cpp

ClientTCP.o: ClientTCP.h ClientTCP.cpp
	$(CC) $(CFLAGS) ClientTCP.cpp

Serveur: main.o VSCOM.o SNPortSerie.o ServeurTCP.o SNIRconversions.o MessageCAN.o JSONFile.o ClientTCP.o
	$(CC) main.o VSCOM.o SNPortSerie.o ServeurTCP.o MessageCAN.o SNIRconversions.o JSONFile.o ClientTCP.o -lpthread -o Serveur 

clean :
	rm -f *.o Serveur

