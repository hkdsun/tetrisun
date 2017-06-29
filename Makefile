TARGET = main
LIBS = -lm -lncurses
CC = gcc
CFLAGS = -g -Wall

SRC_DIR = src
BIN_DIR = bin

.PHONY: default all clean mkbin run

default: mkbin $(TARGET) run
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard src/*.c))
HEADERS = $(wildcard src/*.h)

$(SRC_DIR)/%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $(BIN_DIR)/$@

clean:
	-rm -f *.o
	-rm -rf $(BIN_DIR)

mkbin:
	-mkdir -p $(BIN_DIR)

run:
	-bin/main
