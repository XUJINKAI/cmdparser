all: cmdparser.o
	$(MAKE) -C test
	$(MAKE) -C demo_simple
	$(MAKE) -C demo_git

clean:
	-rm cmdparser.o
	$(MAKE) clean -C test
	$(MAKE) clean -C demo_simple
	$(MAKE) clean -C demo_git

cmdparser.o: cmdparser.c
	gcc -Wall -Wextra -std=gnu99 -c -o $@ $^
