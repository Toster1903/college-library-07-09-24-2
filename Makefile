CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic
SOURCES = main.cpp LibraryItem.cpp Book.cpp Magazine.cpp Reader.cpp Loan.cpp Library.cpp Input.cpp
library: $(SOURCES) $(wildcard *.h)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o library
run: library
	./library
test: library
	python3 tests/test_library.py
clean:
	rm -f library
.PHONY: run test clean
