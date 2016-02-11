CC=gcc -fPIC
ARCHIVE=ar rcs
INCLUDES= -I./src -I./test
SOURCES= $(shell echo src/*.c)
HEADERS= $(shell echo src/*.h)
OBJECTS= $(SOURCES:.c=.o)

default: bench test shared static

bench: benchmark/bench.o $(OBJECTS)
	$(CC) -o benchmark/bench benchmark/bench.o $(OBJECTS) $(INCLUDES)

test: test/test.o $(OBJECTS)
	$(CC) -o test/test test/test.o $(OBJECTS) $(INCLUDES)
	test/test

shared: $(OBJECTS)
	$(CC) -o build/libbkstring.so -shared $(OBJECTS) $(INCLUDES)

static: $(OBJECTS)
	$(ARCHIVE) build/libbkstring.a $(OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) -c $(INCLUDES) $< -o $@

clean:
	-rm -f src/*.o
	-rm -f test/*.o
	-rm -f benchmark/*.o
	-rm -f test/test
	-rm -f benchmark/bench
	-rm -f src/bkstring.so
