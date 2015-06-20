CFLAGS := -Wall -O3

.PHONY: all test asm

all: asm test

a.out: main.cpp
	$(CXX) $(CFLAGS) main.cpp

test: a.out
	./a.out

asm: main.s
main.s: main.cpp
	$(CXX) -S $(CFLAGS) main.cpp

