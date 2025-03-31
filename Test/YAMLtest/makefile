CC=g++ 
CFLAGS=-c -w

all:YamlConf main.o YamlConfigParser.o

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -std=c++11

Config_yaml.o: YamlConfigParser.cpp YamlConfigParser.h
	$(CC) $(CFLAGS) YamlConfigParser.cpp

YamlConf: main.o YamlConfigParser.o
	$(CC) main.o YamlConfigParser.o -o YamlConf

clean:
	rm -f *.o YamlConf