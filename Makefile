CC=gcc -fPIC
ARCHIVE=ar rcs
INCLUDES= -I./src -I./test
SOURCES= $(shell echo src/*.c)
HEADERS= $(shell echo src/*.h)
OBJECTS= $(SOURCES:.c=.o)

default: bench shared static test

# Creates the benchmark utility executable.
bench: benchmark/bench.o $(OBJECTS)
	$(CC) -o benchmark/bench benchmark/bench.o $(OBJECTS) $(INCLUDES)

# Creates a benchmark utility executable to be tested by valgrind.
bench-test: benchmark/bench.o $(OBJECTS)
	$(CC) -g -o benchmark/bench benchmark/bench.o $(OBJECTS) $(INCLUDES)

# Creates and runs the tests for BK String
test: test/test.o $(OBJECTS)
	$(CC) -o test/test test/test.o $(OBJECTS) $(INCLUDES)
	test/test

# Creates shared .so library.
shared: $(OBJECTS)
	$(CC) -o build/libbkstring.so -shared $(OBJECTS) $(INCLUDES)

# Creates static .a library.
static: $(OBJECTS)
	$(ARCHIVE) build/libbkstring.a $(OBJECTS)
	cp $(HEADERS) build

# Compiles the object libraries to be linked during make.
%.o: %.c $(HEADERS)
	$(CC) -c $(INCLUDES) $< -o $@

clean:
	-rm -f src/*.o
	-rm -f test/*.o
	-rm -f benchmark/*.o
	-rm -f test/test
	-rm -f benchmark/bench
	-rm -f src/bkstring.so
	-rm -f build/*
