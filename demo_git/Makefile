CC := gcc
CFLAGS := -Wall -std=gnu11 -I../
APP_NAME := run.exe

SRC_SOURCES := $(shell find $(SRC_DIR) -type f -name '*.c')
SRC_OBJECTS := $(patsubst %.c, build/%.o, $(SRC_SOURCES))

app: $(SRC_OBJECTS) build/cmdparser.o
	$(CC) $(CFLAGS) -o $(APP_NAME) $^

clean:
	-rm -r build *.exe

build/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

build/cmdparser.o: ../cmdparser.c
	@mkdir -p $(@D)
	gcc -Wall -Wextra -std=gnu99 -c -o $@ $^
