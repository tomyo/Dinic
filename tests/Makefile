# La forma normal de usar este Makefile debería ser correr
# "make test" EN EL DIRECTORIO DE ARRIBA, no en este.
TARGET=runner
TARGET_MEM=leaktest
CC=gcc
LDFLAGS=-lcheck `pkg-config --cflags --libs glib-2.0`
CFLAGS+= -I.. -ansi -Wall -Wextra -Wdeclaration-after-statement -Wbad-function-cast -Wcast-qual -Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes -Wno-unused-parameter -Werror -pedantic -g $(LDFLAGS)
TEST_SOURCES=$(shell echo test*.c) run_tests.c
MEM_SOURCES=$(shell echo test*.c) leaktest.c
TEST_OBJECTS= $(TEST_SOURCES:.c=.o)
MEM_OBJECTS= $(MEM_SOURCES:.c=.o)
# Aquí no usamos un wildcard para no traer el main
TESTED_OBJECTS=../edge.o ../network.o

test: $(TARGET)
	./$^

memtest: $(TARGET_MEM)
	# Ver http://live.gnome.org/Valgrind para explicacion de G_SLICE
	G_SLICE=always-malloc valgrind --leak-check=full --show-reachable=yes ./$^

$(TARGET): $(TEST_OBJECTS) $(TESTED_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(TARGET_MEM): $(MEM_OBJECTS) $(TESTED_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(TARGET_MEM) $(OBJECTS) $(MEM_OBJECTS) test.log .depend *~

.depend: *.[ch]
	$(CC) $(CFLAGS) -MM $(SOURCES) >.depend

-include .depend

.PHONY: clean all

