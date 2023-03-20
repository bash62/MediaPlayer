CC = gcc
CFLAGS = -g -Wall
CLIBS = -lm -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

BIN_DIR = ./build
OBJ_DIR = ./obj
SRC_DIR = ./src

OUTPUT = MediaPlayer


OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/mediaplayer.o $(OBJ_DIR)/window.o $(OBJ_DIR)/malwareUtils.o $(OBJ_DIR)/malwareBuilder.o

all: clean MediaPlayer

MediaPlayer: $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUT) $(CLIBS)
	cp -r ./images $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@ $(CLIBS)

clean:
	rm -rf $(OBJS) $(OUTPUT)
