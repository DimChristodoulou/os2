# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g

OBJECTS = tree.o tests.o

tree: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -lm -o tree -lcunit

tree.o: src/tree.c
	$(CC) $(CFLAGS) -c src/tree.c

tests.o: tests.c
	$(CC) $(CFLAGS) -c tests/tests.c

clean:
	rm -fv *.o
	rm -fv tree

cunit:
	sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev