FILES := $(filter-out main.cc,$(wildcard *.cc))
OBJS := $(patsubst %.cc,%.o,$(filter-out t_%, $(FILES)))
TEST := $(patsubst %.cc,%.o,$(filter t_%, $(FILES)))
GTEST_OBJS := gtest-all.o gtest_main.o

GTEST = $(HOME)/build/gtest
TFLAGS = -I$(GTEST)/include
CFLAGS = -Wall -Wextra -g3 -pg `pkg-config --cflags glib-2.0`


all: bruce test

bruce: $(OBJS) main.o
	$(CXX) -o bruce $(OBJS) main.o -pg `pkg-config --libs glib-2.0`

.PHONY: test
test: gtest
	./gtest

gtest: $(TEST) $(GTEST_OBJS) $(OBJS)
	$(CXX) -g3 -o gtest $(TEST) $(GTEST_OBJS) $(OBJS) -lpthread `pkg-config --libs glib-2.0`

main.o: main.cc
	$(CXX) $(CFLAGS) $(INC) -c $<

$(OBJS): %.o: %.cc
	$(CXX) $(CFLAGS) $(INC) -c $<

$(TEST): %.o: %.cc
	$(CXX) $(CFLAGS) $(INC) -c $< $(TFLAGS)

gtest-all.o: $(GTEST)/src/gtest-all.cc
	$(CXX) $(CFLAGS) $(TFLAGS) -I$(GTEST) $(INC) -c $<

gtest_main.o: $(GTEST)/src/gtest_main.cc
	$(CXX) $(CFLAGS) $(TFLAGS) -I$(GTEST) $(INC) -c $<

.PHONY: clean
clean:
	rm -f *.o *.so bruce gtest

