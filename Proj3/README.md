# CSCE 420 Programming Assignment #3: Natural Deduction
There is a shipment of 3 boxes filled with tennis balls. 1 box contains only
yellow tennis balls, 1 box contains only white tennis balls, and 1 box 
contains both yellow and white tennis balls. Each box is definitely 
labeled wrong. 1 ball is drawn from each box and observed. Given the
incorrect labeling and the 3 observations, use propositional logic to
derive the correct labeling of the middle box. Using a parser, my code
reads in a knowledge base (.kb file) depicting a model of the aforementioned
Sammy's Sport Shop World and calls functions for various rules of inference
in order to prove by natural deduction that the middle box (box 2) actually contains
white balls. The sentences of the model include truth assignments according to the
problem description as well as initial facts of a specific instance of box labelling and
observations.

## Compiling and Running
In order to compile and link the program, simply call "make". Then, to run, call
"make run". This program uses no command-line arguments, reading in "sammys.kb"
and writing results to "sammys.txt" by default.

## Results
The sammys.txt file shows the derived sentences from the natural deduction proof.
The last line shows "C2W", depicting that the proof is correct and does indeed
arrive at the conclusion that the middle box contains white balls. The sentences
in this file are formatted like how the sentences in the knowledge based are formatted.
Hence, "(implies (and A B) (not C))" would, for example, mean "A ^ B -> ¬C"

## Constraints
For this project, there are several limiting factors. For one, the deduction function
only works on the specific set of initial facts for the specific instance of the problem.
Secondly, the deduction function manually calls the rules of inference functions (as if
one was writing it out on paper) for these initial facts. Thirdly, a rule of inference
function assumes that the expression(s) you give it in the arguments are appropriate for
that specific rule of inference. There may be unexpected behavior, such as the function
returning nullptr, if otherwise. This project functions mainly to get practice with knowledge
bases, parsing them, rules of inference, and a specific natural deduction proof.
