CC = gcc
CFLAGS = -g -Wall
CLIBS = -lm

BIN_DIR = ./bin
OBJ_DIR = ./obj
SRC_DIR = ./src

OUTPUT = $(BIN_DIR)/virus

OBJS = $(OBJ_DIR)/MediaPlayer.o

all: clean virus

virus: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUT) $(CLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@ $(CLIBS)

clean:
	rm -rf $(OBJS) $(OUTPUT)
