# File name: makefile
# Description: Makefile for project 5
# Name: Dean Orenstein
# Class: CSCE 420
# Date: 04/28/2021

FLAGS = -Wall -c -std=c++11

resolution: resolution.cpp resolution.o
	g++ -o $@ resolution.o

resolution.o: resolution.cpp parser.cpp parser.hpp
	g++ $(FLAGS) $< resolution.cpp parser.cpp

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#