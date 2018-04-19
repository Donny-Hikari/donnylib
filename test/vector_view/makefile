#!gmake

SRC       ?=   src/vector_view_unit_test.cpp
BIN       ?=   bin/test
CFLAG     ?=   -std=c++11

RM        ?=   rm -f
MKDIR     ?=   mkdir -p

.PHONY: build run clean

build:
	$(MKDIR) $(dir $(BIN))
	$(CXX) $(SRC) -o $(BIN) $(CFLAG)

run:
	cd $(dir $(BIN)) && pwd && ./$(notdir $(BIN))

clean:
	$(RM) $(BIN)
