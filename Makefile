TARGET=dinic
CC=gcc
CFLAGS+= -Wall -Wextra -Wdeclaration-after-statement -Wbad-function-cast -Wcast-qual -Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes -Wno-unused-parameter -pedantic -g # -Werror
SOURCES=$(shell echo *.c)
OBJECTS= $(SOURCES:.c=.o)
NETFILE = example.network

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@

clean:
	rm -f $(TARGET) $(OBJECTS) .depend *~
	make -C tests clean

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
