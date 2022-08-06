CC := gcc
CFLAGS := -Wall -std=gnu11 -I../ -g3
BUILD_DIR := build
APP_NAME := $(BUILD_DIR)/run.exe

SRC_SOURCES := $(shell find $(SRC_DIR) -type f -name '*.c')
SRC_OBJECTS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_SOURCES))

all: $(APP_NAME)

test: $(APP_NAME)
	./$(APP_NAME) $(args)

memory: $(APP_NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose \
			--trace-children=yes --error-exitcode=1 \
			./$(APP_NAME) $(args)

clean:
	-rm -r $(BUILD_DIR) *.log

$(APP_NAME): $(SRC_OBJECTS) $(BUILD_DIR)/cmdparser.o
	$(CC) -o $(APP_NAME) $^

$(BUILD_DIR)/%.o : %.c common.h utest.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/cmdparser.o: ../cmdparser.c
	@mkdir -p $(@D)
	gcc $(CFLAGS) -c -o $@ $^
