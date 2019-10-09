
CXXFLAGS = -g -std=c++17

.PHONY : all clean

all : main

to_string.o : to_string.hpp
rules.o : rules.hpp

main : main.o rules.o to_string.o
	g++ $^ -o $@

clean:
	rm -rf main *.o a.out
