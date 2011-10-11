.SUFFIXES:
.SUFFIXES: .t .o .c
OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
TEST := $(patsubst %.t,%.o,$(wildcard *.t))

CFLAGS = -Wall -pedantic -ansi
CC = gcc

all: bruce test

bruce: $(OBJS)
	$(CC) -o bruce $(OBJS)

test: test-run
	$(CC) -o runtest $(TEST)

.c.o:
	$(CC) $(CFLAGS) $(INC) -c $<

.t.o:
	$(CC) $(CFLAGS) $(INC) -c $<

clean:
	rm -f *.o *.so bruce

