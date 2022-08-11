all: build
	$(MAKE) -C test
	$(MAKE) -C demo_simple
	$(MAKE) -C demo_git

test:
	$(MAKE) test -C test
	$(MAKE) memory -C test

clean:
	-rm cmdparser.o
	$(MAKE) clean -C test
	$(MAKE) clean -C demo_simple
	$(MAKE) clean -C demo_git

build:
	gcc -Wall -Wextra -Werror -std=gnu99 -c -o cmdparser.o cmdparser.c
	g++ -Wall -Wextra -Werror -std=c++11 -c -o cmdparser.o cmdparser.c -Wno-missing-field-initializers

.PHONY : all test clean
