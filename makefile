CC = gcc
CFLAGS = -g -Wall
CLIBS = -lm -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

BIN_DIR = ./build
OBJ_DIR = ./build/obj
SRC_DIR = ./src

OUTPUT = $(BIN_DIR)/MediaPlayer



OUTPUTMALWARE = $(BIN_DIR)/MalwareBuilder

OBJSMALWARE =  $(OBJ_DIR)/malwareUtils.o $(OBJ_DIR)/malwareBuilder.o  

OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/mediaplayer.o $(OBJ_DIR)/window.o $(OBJSMALWARE)

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
