CFLAGS := -std=c++11 -Wall -O3

.PHONY: all test asm clean

APP := a.out

all: test

$(APP): main.cpp expr.h matx.h expr_engine.h base.h shape.h utils.h log.h
	$(CXX) $(CFLAGS) main.cpp -o $(APP)

test: $(APP)
	./$(APP)

asm: main.s
main.s: main.cpp
	$(CXX) -S $(CFLAGS) main.cpp

clean:
	rm -f main.s
	rm -f a.out*
