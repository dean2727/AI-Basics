# File name: makefile
# Description: Makefile for project 1
# Name: Dean Orenstein
# Class: CSCE 420 518
# Date: 02/15/2021

FLAGS = -Wall -c -std=c++11
OBJECTS = BlocksworldBFS.o
#OBJECTS = BlocksworldBFS.o node.o state.o

BlocksworldBFS: $(OBJECTS)
	g++ -o $@ $(OBJECTS)

BlocksworldBFS.o: BlocksworldBFS.cpp #node.hpp state.hpp
	g++ $(FLAGS) -c $<

# node.o: node.cpp node.hpp
# 	g++ $(FLAGS) -c $<

# state.o: state.cpp state.hpp
# 	g++ $(FLAGS) -c $<

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#