FILES := $(wildcard *.cc)
OBJS := $(patsubst %.cc,%.o,$(filter-out t_%, $(FILES)))
TEST := $(patsubst %.cc,%.o,$(filter t_%, $(FILES)))
GTEST_OBJS := gtest-all.o gtest_main.o 

GTEST = $(HOME)/build/gtest
TFLAGS = -I$(GTEST)/include
CFLAGS = -Wall -Wextra

all: bruce test

bruce: $(OBJS)
	$(CC) -o bruce $(OBJS)

.PHONY: test
test: gtest
	./gtest

gtest: $(TEST) $(GTEST_OBJS) $(OBJS)
	$(CXX) -o gtest $(TEST) $(GTEST_OBJS) $(OBJS) -lpthread

$(OBJS): %.o: %.cc
	$(CC) $(CFLAGS) $(INC) -c $<

$(TEST): %.o: %.cc
	$(CXX) $(CFLAGS) $(INC) -c $< $(TFLAGS)

gtest-all.o: $(GTEST)/src/gtest-all.cc
	$(CXX) $(CFLAGS) $(TFLAGS) -I$(GTEST) $(INC) -c $<

gtest_main.o: $(GTEST)/src/gtest_main.cc
	$(CXX) $(CFLAGS) $(TFLAGS) -I$(GTEST) $(INC) -c $<

.PHONY: clean
clean:
	rm -f *.o *.so bruce

