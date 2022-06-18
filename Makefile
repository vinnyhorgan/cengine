CC := gcc
CFLAGS := -Iinclude -Ilibs/include -std=c99 #-Wall -Wextra -g -O2
LDFLAGS := -Llibs/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

TARGET := program

BUILD_DIR := build
SRC_DIRS := src libs/src

SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/$(TARGET) run

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(BUILD_DIR)/$(TARGET)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)