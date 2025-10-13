CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -Wpedantic

run: main
	./$^

.PHONY: example
example: main
	echo "3 15 1 11 25 9 x" | ./$^		# k n goal q1 q2 q3

.PHONY: plot
plot: main
	echo "2 20 1 x" | ./$^ > plot.txt
	./plot.sage ./plot.txt ./plot.png

.PHONY: test
test: main
	cd test && ./test.sh


main: main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm main plot.txt plot.png
