# Project 5: Resolution Refutation
The goal of this project is to do logical proofs using resolution refutation algorithm. Such
an algorithm adds the negated query to the KB and repeatedly applies the resolution rule of
inference for clause pairs for a CNF knowledge base until the empty clause is encountered (proof by contradiction).
The heuristic used is the minimum clause length heuristic, which prioritizes pairs of clauses
such the length of one of the literals in the pair is as small as possible. In this project, several
helper functions are defined for the subroutines of resolution() (for resolution refutation), 
including functions to resolve 2 clauses for a resolvent, validate a clause to ensure it is in
the proper form, see if 2 clauses are resolvable, and get a list of literals that appear
as positive in 1 clause and negative in the other. Knowledge bases for the Sammy's Sports Shop
and Wumpus World (4x4) problems are used to test the algorithm, and the output showcases the 
iterations of the algorithm and what new clauses (resolvents) are added to the KB as the proof
progresses, ended by an indication of success or failure.

## Compiling and Running
In order to compile and link the program, simply call "make". Then, to run, call
"./resolution filename clause". The .cnf file (filename) is the knowledge base representing
the constraints for the problem. The clause parameter is the clause we want entailed, in which
the negated clause will be added to the KB at the start of the algorithm. The Sammy's Sports Shop
knowledge base (sammys.cnf) includes the rules for the problem and the initial facts (which must
be surrounded by "(or" ")" in order for the algorithm to work properly. These facts can be adjusted
to prove different different things for different instances of the problem. For the Wumpus World,
a specific instance (wumpusInstance.in) is provided, which the python script (wumpus.py) read in
and created rules for. To create a different instance of the wumpus world, edit the content of
wumpusInstance.in and, if the width and height of the environment is changed, be sure to use
correct "N" when calling "python wumpus.py N > newfile.cnf". Calling this created a new .cnf
file for you to use when running the main algorithm.

## Results
Several .txt files starting with "reso" are provided, which show the output of the program 
for the aforementioned problems. One can create more output files by redirecting the output
on new executions. "reso_sammys.txt" shows the output for the Sammy's problem (21 iterations for success), which entails
C2W. Here is my process for the wumpus world problem, given the inital configuration in the handout:<br>
1. Entail safe11 successfully (286 iterations)<br>
2. Entail pit23 successfully (463 iterations)<br>
3. Entail pit41 successfully (219 iterations)<br>
4. Entail (not wumpus21) successfully (120 iterations)<br>
5. Entail breezy31 successfully (256 iterations)<br>
6. Add the fact "(not pit41)" to the KB<br>
7. Entail "(not safe43)" successfully (509 iterations)<br>
8. Entail wumpus33 successfully (170 iterations)<br>
