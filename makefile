CC=gcc
FLAGS=-W -Wall
POSTFLAGS=-lm

OBJ_DIR=./bin
SRC_DIR=./src
INC_DIR=./include

EXEC=./main
SRC=$(shell find $(SRC_DIR) -name "*.c")
HEAD=$(shell find $(INC_DIR) -name "*.h")
OBJ=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# main build
$(EXEC): $(OBJ)
	$(CC) $(FLAGS) $^ -o $@ $(POSTFLAGS)

# functions
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(EXEC)

.PHONY: clean

# compilation and link
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h
	mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -o $@ -c $< $(POSTFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -o $@ -c $< $(POSTFLAGS)

# dependencies

$(OBJ_DIR)/main.o: $(OBJ_DIR)/game.o

$(OBJ_DIR)/game.o: $(OBJ_DIR)/util.o
