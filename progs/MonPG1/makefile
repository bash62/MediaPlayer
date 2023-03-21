CC = gcc
CFLAGS = -g -Wall
CLIBS = -lm -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

BIN_DIR = ../../bin
LOCAL_BIN_DIR = ./bin

OBJ_DIR = ./obj
SRC_DIR = ./src

OUTPUT = $(BIN_DIR)/MonPG1
LOCAL_OUTPUT = $(LOCAL_BIN_DIR)/MonPG1

OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/window.o $(OBJ_DIR)/calc.o $(OBJ_DIR)/button.o

all: clean_global MonPG1_global

local: clean_local MonPG1_local

MonPG1_global: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUT) $(CLIBS)
	cp -r ./fonts $(BIN_DIR)

MonPG1_local: $(OBJS)
	mkdir -p $(LOCAL_BIN_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(LOCAL_OUTPUT) $(CLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@ $(CLIBS)

clean_global:
	rm -rf $(OBJS) $(OUTPUT)

clean_local:
	rm -rf $(OBJS) $(LOCAL_OUTPUT)