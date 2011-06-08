TARGET=dinic
CC=gcc
CFLAGS+= -ansi -Wall -Wextra -Wdeclaration-after-statement -Wbad-function-cast -Wcast-qual -Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes -Wno-unused-parameter -pedantic -g -O3
SOURCES=$(shell echo *.c)
OBJECTS= $(SOURCES:.c=.o)
HASHOBJECTS=$(shell echo hashtable/*.o)
NETFILE = example.network

all:
	make -C hashtable
	make $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ $(HASHOBJECTS) -o $@

clean:
	rm -f $(TARGET) $(OBJECTS) .depend *~
	make -C tests clean
	make -C hashtable clean

test: $(OBJECTS)
	make -C tests test

memtest: $(OBJECTS)
	make -C tests memtest

run: $(TARGET)
	./$^ < $(NETFILE)

memrun: $(TARGET)
	valgrind --leak-check=full --show-reachable=yes ./$^ < $(NETFILE)

docs:
	doxygen Doxyfile

.depend: *.[ch]
	$(CC) -MM $(SOURCES) >.depend

-include .depend

.PHONY: clean all
