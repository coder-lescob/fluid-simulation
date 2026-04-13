CC := gcc

DEBUG_FLAGS 	:= -g -Wall -Wextra -Wpedantic
RELEASE_FLAGS 	:= 
LD_FLAGS 		:= $$(pkg-config --libs glfw3) $$(pkg-config --libs gl)

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

debug: build
	@gdb $(TARGET)