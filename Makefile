CXX_FLAGS := -std=c++26 -Wall -Wextra -Werror -pedantic -Iincludes -Icatch2

SRC := $(filter-out src/main.cc, $(wildcard src/*.cc))
OBJ := $(SRC:.cc=.o)
TEST_SRC := $(wildcard tests/*.cc)
CATCH_SRC := catch2/catch_amalgamated.cpp
CATCH_OBJ := catch2/catch_amalgamated.o

%.o: %.cc
	$(CXX) $(CXX_FLAGS) -c $< -o $@

bin/exec: $(OBJ) src/main.o
	$(CXX) $(OBJ) src/main.o -o $@

bin/tests: $(OBJ) $(TEST_SRC) $(CATCH_OBJ)
	$(CXX) $(CXX_FLAGS) $(OBJ) $(TEST_SRC) $(CATCH_OBJ) -o $@

.PHONY: tests
tests: bin/tests