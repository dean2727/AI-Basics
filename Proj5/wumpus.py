'''
File name: wumpus.py
Creates all of the rules in CNF for the knowledge base of the Wumpus World problem.
We have a 4x4 cave, with the following coordinate system:
(1,4) (2,4) (3,4) (4,4)
(1,3) (2,3) (3,3) (4,3)
(1,2) (2,2) (3,2) (4,2)
(1,1) (2,1) (3,1) (4,1)
There is a Wumpus in one of these spots. Spots adjacent to pits have breezys, and spots
adjacent to the Wumpus have a stench. We want to find the gold in the cave, traversing
spots that are safe, and then leave the cave once its obtained. If we fall into a pit
or get eaten by the Wumpus, we lose.
(output is meant to be redirected into a .cnf file)
Name: Dean Orenstein
Class: CSCE 420
Date: 04/28/2021
'''

import sys

N = 0

if __name__ == "__main__":
    N = int(sys.argv[1])

    wumpAdj = set()
    for X in range(1, N+1):
        for Y in range(1, N+1):
            
            ''' at most 1 spot can have a wumpus
            e.g. wumpus11 -> ¬wumpusXY (for all XY that is not 11)
            In CNF: ¬wumpus11 v ¬wumpusXY'''
            for otherX in range(1, N+1):
                for otherY in range(1, N+1):
                    if otherX != X or otherY != Y:
                        rule = "(or (not wumpus"+str(X)+str(Y)+") (not wumpus"+str(otherX)+str(otherY)+"))"
                        converse = "(or (not wumpus"+str(otherX)+str(otherY)+") (not wumpus"+str(X)+str(Y)+"))"
                        if converse not in wumpAdj:
                            print(rule)
                            wumpAdj.add(rule)

            ''' room is safe if and only if theres no wumpus or pit.
            e.g. safe11 -> (¬wumpus11 ^ ¬pit11), (¬wumpus11 ^ ¬pit11) -> safe11
            In CNF: ¬safe11 v ¬wumpus11, ¬safe11 v ¬pit11, wumpus11 v pit11 v safe11 '''
            print("(or (not safe"+str(X)+str(Y)+") (not wumpus"+str(X)+str(Y)+"))")
            print("(or (not safe"+str(X)+str(Y)+") (not pit"+str(X)+str(Y)+"))")
            print("(or wumpus"+str(X)+str(Y)+" pit"+str(X)+str(Y)+" safe"+str(X)+str(Y)+")")

            ''' rooms adjacent to wumpus have a stench.
            e.g. wumpus11 -> stenchXY (for all adjacent XY)
            In CNF: ¬wumpus11 v stenchXY

            Also, if there is a stench, then the wumpus might be in one of the adjacent rooms.
            e.g. stench11 -> wumpus12 v wumpus21
            In CNF: ¬stench11 v wumpus12 v wumpus21

            Also, rooms adjacent to pits have a breeze. 
            e.g. pit11 -> breezyXY (for all adjacent XY)
            In CNF: ¬pit11 v breezyXY

            Also, if there is a breeze, then a pit might be in one of the adjacent rooms.
            e.g. breezy11 -> pit12 v pit21
            In CNF: ¬breezy11 v pit12 v pit21 '''

            breeze = "(or (not breezy"+str(X)+str(Y)+") "
            stench = "(or (not stench"+str(X)+str(Y)+") "

            # can get the left and right
            if X > 1 and X < N:
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X+1)+str(Y)+")")
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X-1)+str(Y)+")")
                stench += "wumpus"+str(X+1)+str(Y)+" "
                stench += "wumpus"+str(X-1)+str(Y)+" "
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X+1)+str(Y)+")")
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X-1)+str(Y)+")")
                breeze += "pit"+str(X+1)+str(Y)+" "
                breeze += "pit"+str(X-1)+str(Y)+" "
            # can only get the left
            elif X > 1:
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X-1)+str(Y)+")")
                stench += "wumpus"+str(X-1)+str(Y)+" "
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X-1)+str(Y)+")")
                breeze += "pit"+str(X-1)+str(Y)+" "
            # can only get the right
            elif X < N:
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X+1)+str(Y)+")")
                stench += "wumpus"+str(X+1)+str(Y)+" "
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X+1)+str(Y)+")")
                breeze += "pit"+str(X+1)+str(Y)+" "
            # can get the top and bottom
            if Y > 1 and X < N:
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X)+str(Y+1)+")")
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X)+str(Y-1)+")")
                stench += "wumpus"+str(X)+str(Y+1)+" "
                stench += "wumpus"+str(X)+str(Y-1)+" "
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X)+str(Y+1)+")")
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X)+str(Y-1)+")")
                breeze += "pit"+str(X)+str(Y+1)+" "
                breeze += "pit"+str(X)+str(Y-1)+" "
            # can get the bottom
            elif Y > 1:
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X)+str(Y-1)+")")
                stench += "wumpus"+str(X)+str(Y-1)+" "
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X)+str(Y-1)+")")
                breeze += "pit"+str(X)+str(Y-1)+" "
            # can get the top
            elif Y < N:
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X)+str(Y+1)+")")
                stench += "wumpus"+str(X)+str(Y+1)+" "
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X)+str(Y+1)+")")
                breeze += "pit"+str(X)+str(Y+1)+" "

            stench += ")"
            breeze += ")"            
            print(stench)
            print(breeze)          

    # rules for this specific instance of the wumpus world
    print("# initial facts")
    with open("wumpusInstance.in", "r") as wi:
        # rows specifiying which spots are visited
        for i in range(N):
            line = wi.readline()
            for j in range(N):
                if line[j] == "v":
                    # coordinate system in this loop is a little flipped from wumpus coordinate system
                    X = j+1
                    Y = N-i
                    # if its visited, we know there is no pit or wumpus
                    print("(or (not pit"+str(X)+str(Y)+"))")
                    print("(or (not wumpus"+str(X)+str(Y)+"))")
        
        wi.readline()

        # rows specifying breeze and stench locations
        for i in range(N):
            line = wi.readline()
            for j in range(N):
                X = j+1
                Y = N-i
                # breeze perceived
                if line[j] == "B":
                    print("(or breezy"+str(X)+str(Y)+")")
                    print("(or (not stench"+str(X)+str(Y)+"))")
                # stench perceived
                elif line[j] == "S":
                    print("(or stench"+str(X)+str(Y)+")")
                    print("(or (not breezy"+str(X)+str(Y)+"))")
                # visited, but no breeze or stench perceived
                elif line[j] == "-":
                    print("(or (not stench"+str(X)+str(Y)+"))")
                    print("(or (not breezy"+str(X)+str(Y)+"))")