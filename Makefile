# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g

all: myfind splitter-merger rootNode leafNode

myfind: myfind.o tests.o
	$(CC) $(CFLAGS) tree.o tests.o -lm -o exe/myfind -lcunit

rootNode: rootNode.o
	$(CC) $(CFLAGS) rootNode.o -lm -o exe/rootNode -lcunit

splitter-merger: splitter-merger.o
	$(CC) $(CFLAGS) splitter-merger.o -lm -o exe/splitter-merger -lcunit

leafNode: leafNode.o
	$(CC) $(CFLAGS) leafNode.o -lm -o exe/leafNode -lcunit

myfind.o: src/tree.c
	$(CC) $(CFLAGS) -c src/tree.c

leafNode.o: src/leafNode.c
	$(CC) $(CFLAGS) -c src/leafNode.c

tests.o: tests/tests.c
	$(CC) $(CFLAGS) -c tests/tests.c

splitter-merger.o: src/splitter-merger.c
	$(CC) $(CFLAGS) -c src/splitter-merger.c

rootNode.o: src/rootNode.c
	$(CC) $(CFLAGS) -c src/rootNode.c

clean:
	rm -fv *.o exe/*
	rm -fv tree

cunit:
	sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev