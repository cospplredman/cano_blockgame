
CC=gcc
CFLAGS=-lm -o2 -I src/

PROTOCOL_SOURCES= $(wildcard protocols/*/*.c)
MC_LIB_SOURCES= $(wildcard src/*.c)

all: bin bin/mc_lib.a( $(PROTOCOL_SOURCES:.c=.o) $(MC_LIB_SOURCES:.c=.o) ) bin/srv bin/cli bin/nbt_test

clean:
	rm -rf bin/* src/*.o

bin:
	mkdir bin

bin/srv: app/srv.c bin/mc_lib.a
	${CC} ${CFLAGS} $^ -o $@

bin/cli: app/cli.c bin/mc_lib.a
	${CC} ${CFLAGS} $^ -o $@

bin/nbt_test: app/nbt_test.c bin/mc_lib.a
	${CC} ${CFLAGS} $^ -o $@

.PHONY: all clean
