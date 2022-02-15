files = $(basename $(shell ls *.cpp))

all: $(files)

%: %.cpp                                             
	g++ -std=c++11 -g -o $@ $<

clean:
	rm $(files)
