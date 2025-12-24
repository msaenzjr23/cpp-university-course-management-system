CXX ?= g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O2 -Iinclude

CORE_SRC := src/Student.cpp src/Course.cpp src/University.cpp
APP_SRC := src/main.cpp
CORE_OBJ := $(CORE_SRC:.cpp=.o)
APP_OBJ := $(APP_SRC:.cpp=.o)

APP_TARGET := bin/ucms
TEST_TARGET := bin/tests

all: $(APP_TARGET)

$(APP_TARGET): $(CORE_OBJ) $(APP_OBJ) | bin
	$(CXX) $(CXXFLAGS) -o $@ $(CORE_OBJ) $(APP_OBJ)

$(TEST_TARGET): $(CORE_OBJ) tests/test_university.cpp | bin
	$(CXX) $(CXXFLAGS) -o $@ $(CORE_OBJ) tests/test_university.cpp

bin:
	mkdir -p bin

clean:
	rm -f $(CORE_OBJ) $(APP_OBJ) $(APP_TARGET) $(TEST_TARGET)

run: $(APP_TARGET)
	./$(APP_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: clean run test all
