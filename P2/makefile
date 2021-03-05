# File name: makefile
# Description: Makefile for project 2
# Name: Dean Orenstein
# Class: CSCE 420 518
# Date: 03/05/2021

FLAGS = -Wall -c -std=c++11

BlocksworldAstar: BlocksworldAstar.cpp BlocksworldAstar.o
	g++ -o $@ BlocksworldAstar.o

BlocksworldAstar.o: BlocksworldAstar.cpp node.cpp node.hpp state.cpp state.hpp
	g++ $(FLAGS) $< node.cpp state.cpp

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#