CC = g++ --std=c++11
DEBUG = -g
CFLAGS = -Wall -c
LFLAGS = -Wall

SRC_DIR = src
BIN_DIR = bin
INCL_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/udp_wizard.o $(OBJ_DIR)/hub.o
MAIN = $(BIN_DIR)/udp_wizard

$(MAIN): $(OBJECTS)
	$(CC) $(LFAGS) $(OBJECTS) -o $(MAIN)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCL_DIR)/udp_wizard.h $(INCL_DIR)/hub.h
	$(CC) $(CFLAGS) $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/udp_wizard.o: $(SRC_DIR)/udp_wizard.cpp $(INCL_DIR)/udp_wizard.h $(INCL_DIR)/exceptions.h $(INCL_DIR)/message_structs.h
	$(CC) $(CFLAGS) $(SRC_DIR)/udp_wizard.cpp -o $(OBJ_DIR)/udp_wizard.o

$(OBJ_DIR)/hub.o: $(SRC_DIR)/hub.cpp $(INCL_DIR)/hub.h $(INCL_DIR)/udp_wizard.h $(INCL_DIR)/message_structs.h
	$(CC) $(CFLAGS) $(SRC_DIR)/hub.cpp -o $(OBJ_DIR)/hub.o
