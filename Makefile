CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -Wpedantic

run: main
	./$^

example: main
	echo "3 15 1 11 25 9 x" | ./$^		# k n goal q1 q2 q3

main: main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
