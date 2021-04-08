# File name: makefile
# Description: Makefile for project 4
# Name: Dean Orenstein
# Class: CSCE 420
# Date: 04/09/2021

FLAGS = -Wall -c -std=c++11

DPLL: DPLL.cpp DPLL.o
	g++ -o $@ DPLL.o

DPLL.o: DPLL.cpp parser.cpp parser.hpp
	g++ $(FLAGS) $< DPLL.cpp parser.cpp

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#