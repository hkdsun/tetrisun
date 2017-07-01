TARGET = main
LIBS = -lm -lncurses
CC = gcc
CFLAGS = -g -Wall -std=c11

SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = build

.PHONY: default all clean mkbin run grind

default: mkbin $(TARGET)
all: default

OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c))
HEADERS = $(wildcard $(SRC_DIR)/*.h)

.PRECIOUS: $(TARGET) $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $(BIN_DIR)/$@

clean:
	-rm -f *.o
	-rm -rf $(BIN_DIR) $(OBJ_DIR)

mkbin:
	-mkdir -p $(BIN_DIR)
	-mkdir -p $(OBJ_DIR)

grind: mkbin $(TARGET)
	-valgrind --leak-check=full bin/main 2>debug

run: mkbin $(TARGET)
	-bin/main 2>debug
