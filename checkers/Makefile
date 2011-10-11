.SUFFIXES:
.SUFFIXES: .c .cc .o
OBJS := $(patsubst %.c,%.o,$(wildcard [^t][^_]*.c))
TEST := $(patsubst %.cc,%.o,$(wildcard *.cc)) gtest-all.o gtest_main.o

GTEST = $(HOME)/build/gtest
TFLAGS = -I$(GTEST)/include
CFLAGS = -Wall -Wextra

all: bruce test

bruce: $(OBJS)
	$(CC) -o bruce $(OBJS)

.PHONY: test
test: gtest
	./gtest

gtest: $(TEST)
	$(CXX) -o gtest $(TEST) -lpthread

gtest-all.o: $(GTEST)/src/gtest-all.cc
	$(CXX) $(CFLAGS) $(TFLAGS) -I$(GTEST) $(INC) -c $<

gtest_main.o: $(GTEST)/src/gtest_main.cc
	$(CXX) $(CFLAGS) $(TFLAGS) -I$(GTEST) $(INC) -c $<

.c.o:
	$(CC) $(CFLAGS) $(INC) -c $<

.cc.o:
	$(CXX) $(CFLAGS) $(INC) -c $< $(TFLAGS)

.PHONY: clean
clean:
	rm -f *.o *.so bruce

