#!gmake

RM        ?=   rm -f
SRC       ?=   src/*.cpp
BIN       ?=   bin/test
CFLAG     ?=   -std=c++17

MKDIR     ?=   mkdir -p

build:
	$(MKDIR) $(dir $(BIN))
	$(CXX) $(SRC) -o $(BIN) $(CFLAG)

run:
	cd $(dir $(BIN)) && pwd && ./$(notdir $(BIN))

.PHONY: clean

clean:
	$(RM) $(BIN)
