.PHONY: all debug test

export CXX=g++
export CXXFLAGS=-Wall -Werror -pedantic -std=c++11

all:
	make -C src all

debug:
	make -C src debug

clean:
	make -C src clean
