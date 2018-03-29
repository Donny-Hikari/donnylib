#!gmake

RM        ?=   rm -f
SRC       ?=   test_filesystem.cpp
BIN       ?=   bin/test
CFLAG     ?=   -lstdc++ -std=c++11

MKDIR     ?=   mkdir -p

build:
	$(MKDIR) $(dir $(BIN))
	$(CC) $(SRC) -o $(BIN) $(CFLAG)

run:
	cd $(dir $(BIN)) && pwd && ./$(notdir $(BIN))

.PHONY: clean

clean:
	$(RM) $(BIN)
