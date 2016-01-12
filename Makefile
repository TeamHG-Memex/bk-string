CC=gcc
INCLUDES= -I./src -I../src -I./ -I./test
SRCS= src/bknode.c src/bkslist.c src/bkstring.c src/bkutil.c
DEPS= src/bkstring.h src/bknode.h src/bkslist.h src/bkutil.h
OBJECTS= $(SRCS:.c=.o)

default: bench test

bench: benchmark/bench.o $(OBJECTS)
	$(CC) -o benchmark/bench benchmark/bench.o $(OBJECTS) $(INCLUDES)

test: test/test.o $(OBJECTS)
	$(CC) -o test/test test/test.o $(OBJECTS) $(INCLUDES)

%.o: %.c $(DEPS)
		$(CC) -c $(INCLUDES) $< -o $@

clean:
	-rm -f src/*.o
	-rm -f test/*.o
