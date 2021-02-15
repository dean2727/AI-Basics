# CSCE 420 Programming Assignment #1: Blocksworld BFS Search
The Blocksworld problem is an AI problem defined as follows: given an initial
configuration of blocks, transform this state to a desired goal state of blocks
by finding a sequence of block moves that get us to that state. This program aims
to do just that, where each block is a capitalized letter of the alphabet. It
takes as input a file that specifies the initial and final states of the problem,
and it uses the breadth-first search algorithm to find a path to the goal state.
The output includes an indication of success, and if so, the number of iterations
performed by the algorithm, the depth of the search space, and the max queue size
encountered for the frontier nodes. Nodes represent possible states in the
problem given the constraints.

## Compiling and Running
In order to test the program, you can either use 1 of the 3 given .bwp files or
create your own randomly generated test case file using blockgen.py. Run
"python blockgen.py <S:int> <B:int> <N:int>", where S is the number of stacks,
B is the number of blocks, and N is the number of scrambling steps/moves, and
redirect the output to a .bwp file. To compile, run "make". To run the program,
run "./BlocksworldBFS <file>", where file is a .bwp file needed to set up the
problem.

## Results
The RESULTS.txt file show the results from running the program on the 3 given
test cases (each test case separated by a blank line). If you want to create your
own output file, redirect the output of the program into it.

## Constraints
Currently, the algorithm halts after a max of 100,000 iterations in the algorithm.
If this happens, then the output will indicate a failure since the goal node was
not yet found. Additionally, the program will only be able to solve problems that
require about 7 moves or less, since a large enough branching factor will cause
drastic time increase.
