CC := gcc

DEBUG_FLAGS 	:= -g -Wall -Wextra -Wpedantic
RELEASE_FLAGS 	:= 
LD_FLAGS 		:= 

BUILD_DIR 	:= build
SRC_DIR		:= src

SRC    := $(wildcard $(SRC_DIR)/*.c)
TARGET := $(BUILD_DIR)/fluid-simulation

.PHONY: build
build:
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(DEBUG_FLAGS) $(SRC) -o $(TARGET) $(LD_FLAGS)

run: build
	@$(TARGET) $(ARGS)