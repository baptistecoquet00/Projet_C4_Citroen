CC=g++
CFLAGS=-c -Wall -std=c++17 -I$(INCLUDE_DIR)
BIN_DIR=bin
SRC_DIR=src
INCLUDE_DIR=include
OBJS = $(BIN_DIR)/main.o \
       $(BIN_DIR)/VSCOM.o \
       $(BIN_DIR)/SNPortSerie.o \
       $(BIN_DIR)/ServeurTCP.o \
       $(BIN_DIR)/SNIRconversions.o \
       $(BIN_DIR)/MessageCAN.o \
       $(BIN_DIR)/JSONFile.o \
       $(BIN_DIR)/ClientTCP.o \
       $(BIN_DIR)/YamlConfigParser.o

all: create_dir ServeurFinal

create_dir:
	@mkdir -p $(BIN_DIR)

install:
	cp config_serveur.yml $(BIN_DIR)/

ServeurFinal: $(OBJS)
	$(CC) $(OBJS) -lpthread -o ServeurFinal
	
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.h
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -o $(BIN_DIR)/main.o

clean:
	rm -f $(BIN_DIR)/*.o ServeurFinal