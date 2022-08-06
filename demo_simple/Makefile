CC := gcc
CFLAGS := -Wall -std=gnu11 -I../ -g3
APP_NAME := run.exe

app: main.o cmdparser.o
	$(CC) -o $(APP_NAME) $^

clean:
	-rm *.exe *.o

main.o : main.c
	$(CC) $(CFLAGS) -c -o $@ $<

cmdparser.o: ../cmdparser.c
	gcc $(CFLAGS) -c -o $@ $^
