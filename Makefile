.PHONY: all test

all: test

a.out: main.cpp
	$(CXX) main.cpp

test: a.out
	./a.out
