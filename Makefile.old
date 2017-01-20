CC=  g++-4.9 --std=c++11
DEBUG = -g
CFLAGS = -c -Wall $(DEBUG)
LFAGS = -Wall $(DEBUG)

SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj
INCL_DIR = include

SENDER_OBJS = $(OBJ_DIR)/sender.o
RECEIVER_OBJS = $(OBJ_DIR)/receiver.o
SENDER = $(BIN_DIR)/sender
RECEIVER = $(BIN_DIR)/receiver

ALL: $(SENDER) $(RECEIVER)

$(SENDER): $(SENDER_OBJS)
	$(CC) $(LFLAGS) $(SENDER_OBJS) -o $(SENDER)

$(RECEIVER): $(RECEIVER_OBJS)
	$(CC) $(LFLAGS) $(RECEIVER_OBJS) -o $(RECEIVER)

$(OBJ_DIR)/sender.o: $(SRC_DIR)/sender.cpp
	$(CC) $(CFLAGS) $(SRC_DIR)/sender.cpp -o $(OBJ_DIR)/sender.o

$(OBJ_DIR)/receiver.o: $(SRC_DIR)/receiver.cpp
	$(CC) $(CFLAGS) $(SRC_DIR)/receiver.cpp -o $(OBJ_DIR)/receiver.o

