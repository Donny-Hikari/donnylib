
SRCDIR = src
BINDIR = bin
CFLAGS = -g --std=c++11 -I../../../

.PHONY: all range polynomial piecewise_polynomial makebin clean

all: range polynomial piecewise_polynomial

makebin:
	mkdir -p $(BINDIR)

range: makebin $(SRCDIR)/range.cpp
	$(CXX) $(SRCDIR)/range.cpp -o $(BINDIR)/test_range $(CFLAGS)
	cd $(BINDIR) && ./test_range

polynomial: makebin $(SRCDIR)/polynomial.cpp
	$(CXX) $(SRCDIR)/polynomial.cpp -o $(BINDIR)/test_polynomial $(CFLAGS)
	cd $(BINDIR) && ./test_polynomial

piecewise_polynomial: makebin $(SRCDIR)/piecewise_polynomial.cpp
	$(CXX) $(SRCDIR)/piecewise_polynomial.cpp -o $(BINDIR)/test_piecewise_polynomial $(CFLAGS)
	cd $(BINDIR) && ./test_piecewise_polynomial

clean:
	rm -f $(BINDIR)
