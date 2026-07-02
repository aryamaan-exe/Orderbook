CXX=clang++
INCLUDES=-Iincludes -Icatch2
CXX_FLAGS=-std=c++26 -Wall -Wextra -Werror -pedantic $(INCLUDES)

SRC := $(wildcard src/*.cc)
CATCH_SRC := catch2/catch_amalgamated.cpp
OBJ := $(SRC:.cc=.o)
TESTS := $(wildcard tests/*.cc)

bin/exec: $(OBJ)
	$(CXX) $(OBJ) -o $@

bin/tests: $(TESTS)
	$(CXX) $(CXX_FLAGS) $(TESTS) $(CATCH_SRC) -o $@

src/%.o: src/%.cc
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -f src/*.o bin/exec

exec: bin/exec

tests: bin/tests

.PHONY: clean exec