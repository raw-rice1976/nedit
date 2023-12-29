SRC_DIR = src
BUILD_DIR = build
FLAGS = -ggdb -o
OUTPUT = nedit
CC = gcc

main: $(SRC_DIR)/draw.c $(SRC_DIR)/main.c $(SRC_DIR)/misc.c $(SRC_DIR)/IO.c
	@if test -d $(BUILD_DIR); then echo "build directory exists"; else echo "creating build directory" && mkdir $(BUILD_DIR); fi
	@echo "compiling."
	$(CC) $(FLAGS) $(BUILD_DIR)/$(OUTPUT) $(SRC_DIR)/main.c $(SRC_DIR)/draw.c $(SRC_DIR)/misc.c $(SRC_DIR)/IO.c
