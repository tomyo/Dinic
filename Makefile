TARGET=bfsNet
CC=gcc
CFLAGS+= -Wall -ansi -Wextra -Wdeclaration-after-statement -Wbad-function-cast -Wcast-qual -Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes -Wno-unused-parameter -Werror -pedantic -g `pkg-config --cflags glib-2.0`
LDFLAGS=`pkg-config --cflags --libs glib-2.0`
SOURCES=$(shell echo *.c)
OBJECTS= $(SOURCES:.c=.o)
NETFILE = example.network

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

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
	# Hay que definir la variable G_SLICE por que el uso interno de alocacion de memoria
	# de la libreria GLIB hace que valgrind diga que hay memory leaks cuando no los hay
	G_SLICE=always-malloc valgrind --leak-check=full --show-reachable=yes ./$^ < $(NETFILE)

docs:
	doxygen Doxyfile

.depend: *.[ch]
	$(CC) -MM $(SOURCES) >.depend

-include .depend

.PHONY: clean all
