CC = gcc
CFLAGS = -g -Wall
CLIBS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lm -lpthread

BIN_DIR = ../../bin
LOCAL_BIN_DIR = ./bin

OBJ_DIR = ./obj
SRC_DIR = ./src

OUTPUT_NAME = $(BIN_DIR)/MonPG5
LOCAL_OUTPUT_NAME = $(LOCAL_BIN_DIR)/MonPG5

OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/bonus.o $(OBJ_DIR)/game.o $(OBJ_DIR)/window.o $(OBJ_DIR)/player.o $(OBJ_DIR)/map.o $(OBJ_DIR)/ghost.o 

all: clean_global MonPG5_global

local: clean_local MonPG5_local

MonPG5_global: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUT) $(CLIBS)
	cp -r ./data $(BIN_DIR)
	cp -r ./assets $(BIN_DIR)

MonPG5_local: $(OBJS)
	mkdir -p $(LOCAL_BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(LOCAL_OUTPUT) $(CLIBS)
	cp -r ./data $(LOCAL_BIN_DIR)
	cp -r ./assets $(LOCAL_BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@ $(CLIBS)

clean_global:
	rm -rf $(OBJS) $(OUTPUT)

clean_local:
	rm -rf $(OBJS) $(LOCAL_OUTPUT)