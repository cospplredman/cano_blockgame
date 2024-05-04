
CC=gcc
CFLAGS=-lm -o2 -I src/

all: bin/mc_lib.a(src/mc_dt.o src/mc_pkt.o src/mc_nbt.o) bin/srv bin/cli bin/nbt_test

clean:
	rm -rf bin/* src/*.o

bin/srv: app/srv.c bin/mc_lib.a
	${CC} ${CFLAGS} $^ -o $@

bin/cli: app/cli.c bin/mc_lib.a
	${CC} ${CFLAGS} $^ -o $@

bin/nbt_test: app/nbt_test.c bin/mc_lib.a
	${CC} ${CFLAGS} $^ -o $@

.PHONY: all clean
