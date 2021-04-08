'''
File name: queens.py
Creates all of the rules in CNF for the knowledge base
of an N queens problem, writing to standard output
(output is meant to be redirected into a .cnf file)
Name: Dean Orenstein
Class: CSCE 420
Date: 04/09/2021
'''

import sys

N = 0

def newRule(l1, l2):
    print("(or (not " + l1 + ") (not " + l2 + "))")
    pass

if __name__ == "__main__":
    N = int(sys.argv[1])

    board = []
    for i in range(N):
        row = [False for j in range(N)]
        board.append(row)
    
    # There needs to be at least 1 queen in each column
    # e.g. for N = 3: Q11 v Q12 v Q13
    for c in range(N):
        colRule = "(or "
        for r in range(N):
            colRule += "Q"+str(c+1)+str(r+1)+" "
        colRule += ")"
        print(colRule)

    # Rules for queens not being able to attach each other
    # e.g. for N = 3: Q11 -> ¬Q12 ^ ¬Q13 ^ ¬Q21 ^ ¬Q31 ^ ¬Q22 ^ ¬Q33
    # => separate clauses: {¬Q11, ¬Q12}, {¬Q11, ¬Q13}, {¬Q11, ¬Q21}, {¬Q11, ¬Q31}, {¬Q11, ¬Q22}, {¬Q11, ¬Q33}
    # Loop through each spot on the board, adding horizontal, vertical, and diagonal restrictions (ignore
    # restrictions for anything directly left or above the current tile since these were already added)
    for r in range(N):
        for c in range(N):
            thisQueenLiteral = "(not Q"+str(c+1)+str(r+1)+")"

            # Look to the right
            for cc in range(c+1, N):
                print("(or "+thisQueenLiteral+" (not Q"+str(cc+1)+str(r+1)+"))")

            # Look below
            for rr in range(r+1, N):
                print("(or "+thisQueenLiteral+" (not Q"+str(c+1)+str(rr+1)+"))")
            
            # In diagonal if abs(difference of r) = abs(difference of c)
            for rr in range(r+1, N):
                # Look to the bottom right
                for cc in range(c+1, N):
                    if abs(r-rr) == abs(c-cc):
                        print("(or "+thisQueenLiteral+" (not Q"+str(cc+1)+str(rr+1)+"))")
                # Look to the bottom left (loop the other way)
                if c >= 1:
                    for cc in range(c-1, -1, -1):
                        if (abs(r-rr) == abs(c-cc)):
                            print("(or "+thisQueenLiteral+" (not Q"+str(cc+1)+str(rr+1)+"))")
            

    

    

    
