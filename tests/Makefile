# La forma normal de usar este Makefile debería ser correr
# "make test" EN EL DIRECTORIO DE ARRIBA, no en este.
TARGET=runner
TARGET_MEM=leaktest
CC=gcc
LDFLAGS=-lcheck
CFLAGS+= -I.. -ansi -Wall -Wextra -Wdeclaration-after-statement -Wbad-function-cast -Wcast-qual -Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes -Wno-unused-parameter -Werror -pedantic -g
TEST_SOURCES=$(shell echo test*.c) run_tests.c
MEM_SOURCES=$(shell echo test*.c) leaktest.c
TEST_OBJECTS= $(TEST_SOURCES:.c=.o)
HASHOBJECTS=$(shell echo ../hashtable/*.o)
MEM_OBJECTS= $(MEM_SOURCES:.c=.o)
# Aquí no usamos un wildcard para no traer el main
TESTED_OBJECTS=../edge.o ../network.o ../slist.o ../queue.o ../dinic.o

test: $(TARGET)
	./$^

memtest: $(TARGET_MEM)
	valgrind --leak-check=full --show-reachable=yes ./$^

$(TARGET): $(TEST_OBJECTS) $(TESTED_OBJECTS)
	make -C ../hashtable
	$(CC) $^ $(HASHOBJECTS) -o $@ $(LDFLAGS)

$(TARGET_MEM): $(MEM_OBJECTS) $(TESTED_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(TARGET_MEM) $(TEST_OBJECTS) $(MEM_OBJECTS) test.log .depend *~

.depend: *.[ch]
	$(CC) $(CFLAGS) -MM $(SOURCES) >.depend

-include .depend

.PHONY: clean all

