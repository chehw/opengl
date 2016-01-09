TARGET=chapter-3.2

CC=gcc
LINKER=gcc -o
CFLAGS=-Wall -g
CFLAGS=$(shell pkg-config --cflags gl)
CFLAGS+=$(shell pkg-config --cflags glut)
CFLAGS+=$(shell pkg-config --cflags glew)

LIBS=$(shell pkg-config --libs glew)
LIBS+=-lglut

SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := bin

SOURCES :=$(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: do_init $(BIN_DIR)/$(TARGET)
	
	
$(BIN_DIR)/$(TARGET): $(OBJECTS)
	$(LINKER) $@ $(OBJECTS) $(LIBS)

$(OBJECTS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: do_init clean
do_init:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)
	
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/$(TARGET)
	rmdir $(OBJ_DIR) $(BIN_DIR)
