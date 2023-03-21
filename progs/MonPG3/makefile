CC = gcc
CFLAGS = -g -Wall
CLIBS = -lm -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

BIN_DIR = ../../bin
LOCAL_BIN_DIR = ./bin

OBJ_DIR = ./obj
SRC_DIR = ./src

OUTPUT = $(BIN_DIR)/MonPG3
LOCAL_OUTPUT = $(LOCAL_BIN_DIR)/MonPG3

OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/window.o $(OBJ_DIR)/human_bench.o

all: clean_global MonPG3_global

local: clean_local MonPG3_local

MonPG3_global: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUT) $(CLIBS)
	cp -r ./fonts $(BIN_DIR)

MonPG3_local: $(OBJS)
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