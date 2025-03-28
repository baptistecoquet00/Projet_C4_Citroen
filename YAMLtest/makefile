CC=g++ 
CFLAGS=-c -w

all:YamlConf main.o Config_yaml.o

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -std=c++11

Config_yaml.o: Config_yaml.cpp Config_yaml.h
	$(CC) $(CFLAGS) Config_yaml.cpp

YamlConf: main.o Config_yaml.o
	$(CC) main.o Config_yaml.o -o YamlConf

clean:
	rm -f *.o YamlConf