# Project 4: DPLL
Satisfiability problems, such as the map coloring problem and N queens problem, 
can be solved by using the DPLL (Davis-Putnam-Logemann-Loveland) algorithm. This
project aims to do just that. It will read in the knowledge base, which is in a .cnf
file and is assumed to have the CNF form of the rules that govern the problem in question.
It then finds the unique symbols to assign in the model and executes DPLL. The unit
clause heuristic is used, which speeds up execution by searching for clauses where
every literal is false except 1 unassigned literal (thus forcing an assingment on the
unassigned literal). The algorithm can be executed with or without this heuristic
depending on if the -unit flag is set. The knowledge base is already supplied for 
the map coloring problem (including mapcolor2.cnf, which has facts that conflict with
a coloring constraint) and the 6 queens problem.

## Compiling and Running
In order to compile and link the program, simply call "make". Then, to run, call
"./DPLL filename -unit". The cnf file (filename) is the knowledge base representing
the constraints for the problem. The optional UC flag (-unit) determines if the unit-clause
heuristic is to be used. Type "-unit" to turn off the heuristic, and dont type
anything otherwise. New knowledge bases for the (Australia) map coloring problem
and N queens problems can be generated. For map coloring, run "python mapcolor.py > filename",
which will redirect the output of the script (rules) into the .cnf file. For N queens,
run "python queens.py N > filename", where N is the width & height of the NxN board
used for the N queens problem, and the output is similarly redirected into the .cnf file.

## Results
There are 6 .txt files showing the output of the program from running DPLL on the 
map coloring problem (with and without the conflicting facts, each with and without
using the unit-clause heuristic) and the 6 queens problem (with and without the heuristic).
RESULTS.txt shows a summary (in table format) of each of these results, including whether
a satisfiable model could be found, what the model is, the number of DPLL calls, and the
number of times backtracking occured.
