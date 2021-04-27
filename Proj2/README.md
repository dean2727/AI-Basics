# Project 2: Blocksworld A* Search
The Blocksworld problem is an AI problem defined as follows: given an initial
configuration of blocks, transform this state to a desired goal state of blocks
by finding a sequence of block moves that get us to that state. This program aims
to do just that, where each block is a capitalized letter of the alphabet. It
takes as input a file that specifies the initial and final states of the problem.
In contrast to project 1, which uses breadth-first search, the algorithm used here
is best-first search with heuristic (A* search). The output includes an indication of
success, and if so, the number of iterations performed by the algorithm, the depth of
the search space, and the max queue size encountered for the frontier nodes. Nodes
represent possible states in the problem given the constraints. The current 
heuristic used in this algorithm adds 2 for every block that’s not currently directly
on top of the block on which it has to be in the goal state, or adds 2 if there is such
a block below it. Or, if a block is on the ground but in the wrong stack, add 2. This
heuristic can solve a few instances of blocksworld problems with 20 blocks (albeit in
many iterations), and it was able to solve 30/45 test cases. I describe some other heuristic
ideas/attempts in the results file, some of which actually do perform very nicely when
implemented properly.

## Compiling and Running
In order to test the program, you can either use 1 of the 3 given .bwp files or
create your own randomly generated test case file using blockgen.py. Run
"python blockgen.py <S:int> <B:int> <N:int>", where S is the number of stacks,
B is the number of blocks, and N is the number of scrambling steps/moves, and
redirect the output to a .bwp file. To compile, run "make". To run the program,
run "./BlocksworldBFS <file>", where file is a .bwp file needed to set up the
problem.

## Results
The RESULTS.docx file explains the various attempted heuristics, an illustration of the
chosen heuristic, and a performance summary using the heuristic. The STATISTICS.xlsx file
depicts the output from the algorithm across 45 test cases, each of varying number of stacks
and number of blocks. If you want to create your own output file, redirect the output of the
program into it.

## Constraints
Currently, the algorithm halts after a max of 100,000 iterations in the algorithm.
If this happens, then the output will indicate a failure since the goal node was
not yet found. In contrast to the limitation of around 7 moves in project 1 (BFS),
the heuristic allows for solving of larger blocksworld problems and, hence, many
more moves.
