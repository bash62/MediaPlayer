CC = gcc
CFLAGS = -g -Wall
CLIBS = -lm -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

BIN_DIR = ./bin
OBJ_DIR = ./obj
SRC_DIR = ./src

OUTPUT = $(BIN_DIR)/MediaPlayer

OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/mediaplayer.o $(OBJ_DIR)/window.o 


OUTPUTMALWARE = $(BIN_DIR)/MalwareBuilder

OBJSMALWARE =  $(OBJ_DIR)/malwareBuilder.o  $(OBJ_DIR)/malwareUtils.o 

all: clean MediaPlayer

MediaPlayer: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUT) $(CLIBS)

MalwareBuilder:	$(OBJSMALWARE)
	$(CC) $(CFLAGS) $(OBJSMALWARE) -o $(OUTPUTMALWARE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@ $(CLIBS)

clean:
	rm -rf $(OBJS) $(OUTPUT)
	rm -rf $(OBJSMALWARE) $(OUTPUTMALWARE)
