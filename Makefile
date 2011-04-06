TARGET=befese
CC=gcc
CFLAGS+= -Wall -ansi -Wextra -Wdeclaration-after-statement -Wbad-function-cast -Wcast-qual -Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes -Wno-unused-parameter -Werror -pedantic -g `pkg-config --cflags glib-2.0`
LDFLAGS=`pkg-config --libs glib-2.0`
SOURCES=$(shell echo *.c)
OBJECTS= edge.o #$(SOURCES:.c=.o)

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

.depend: *.[ch]
	$(CC) -MM $(SOURCES) >.depend

-include .depend

.PHONY: clean all
