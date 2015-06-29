CFLAGS := -std=c++11 -Wall -O3

.PHONY: all test asm clean

all: asm test

a.out: main.cpp expr.h matx.h
	$(CXX) $(CFLAGS) main.cpp

test: a.out
	./a.out

asm: main.s
main.s: main.cpp
	$(CXX) -S $(CFLAGS) main.cpp


clean:
	rm -f main.s
	rm -f a.out
