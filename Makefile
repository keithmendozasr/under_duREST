.PHONY: all debug test

export CXX=g++ --version
export CXXFLAGS=-Wall -Werror -pedantic -std=c++14

all:
	make -C src all

debug:
	make -C src debug

clean:
	make -C src clean
