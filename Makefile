CC = g++-4.9 --std=c++11
DEBUG = -g
CFLAGS = -Wall -c
LFLAGS = -Wall

SRC_DIR = src
BIN_DIR = bin
INCL_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/udp_bcaster.o
MAIN = $(BIN_DIR)/udp_bcaster

$(MAIN): $(OBJECTS)
	$(CC) $(LFAGS) $(OBJECTS) -o $(MAIN)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCL_DIR)/udp_bcaster.h
	$(CC) $(CFLAGS) $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/udp_bcaster.o: $(SRC_DIR)/udp_bcaster.cpp $(INCL_DIR)/udp_bcaster.h $(INCL_DIR)/exceptions.h
	$(CC) $(CFLAGS) $(SRC_DIR)/udp_bcaster.cpp -o $(OBJ_DIR)/udp_bcaster.o
