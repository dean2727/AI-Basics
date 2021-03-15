# File name: makefile
# Description: Makefile for project 1
# Name: Dean Orenstein
# Class: CSCE 420 518
# Date: 02/15/2021

FLAGS = -Wall -c -std=c++11

BlocksworldBFS: BlocksworldBFS.cpp BlocksworldBFS.o
	g++ -o $@ BlocksworldBFS.o

BlocksworldBFS.o: BlocksworldBFS.cpp node.cpp node.hpp state.cpp state.hpp
	g++ $(FLAGS) $< node.cpp state.cpp

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#