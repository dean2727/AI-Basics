# File name: makefile
# Description: Makefile for project 3
# Name: Dean Orenstein
# Class: CSCE 420
# Date: 03/22/2021

FLAGS = -Wall -c -std=c++11

# sammys: sammys.cpp sammys.o
# 	g++ -o $@ sammys.o

# sammys.o: sammys.cpp NatDed.cpp parser.cpp parser.hpp
# 	g++ $(FLAGS) $< sammys.cpp NatDed.cpp parser.cpp

test: test.cpp test.o
	g++ -o $@ test.o

test.o: test.cpp NatDed.cpp parser.cpp parser.hpp
	g++ $(FLAGS) $< NatDed.cpp parser.cpp

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#

run:
	./test