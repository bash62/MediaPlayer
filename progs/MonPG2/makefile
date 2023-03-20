CC = gcc
CFLAGS = -g -Wall
CLIBS = -lm -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

BIN_DIR = ../../bin
OBJ_DIR = ./obj
SRC_DIR = ./src

OUTPUT = $(BIN_DIR)/MonPG2
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/window.o $(OBJ_DIR)/human_bench.o

all: clean MonPG2

MonPG2: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUT) $(CLIBS)
	cp -r ./fonts $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@ $(CLIBS)

clean:
	rm -rf $(OBJS) $(OUTPUT)