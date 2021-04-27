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

    # rule for there being a wumpus in at least 1 spot
    wumpusOneSpot = "(or "

    wumpAdj = set()
    for X in range(1, N+1):
        for Y in range(1, N+1):
            wumpusOneSpot += "wumpus"+str(X)+str(Y)+" "
            
            ''' at most 1 spot can have a wumpus
            e.g. wumpus11 -> ¬wumpusXY (for all XY that is not 11)
            In CNF: ¬wumpus11 -> ¬wumpusXY '''
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
            Also, rooms adjacent to pits have a breezy. 
            e.g. pit11 -> breezyXY (for all adjacent XY)
            In CNF: ¬pit11 v breezyXY
            Also, the inverse is true. rooms without wumpus have adjacent rooms with no stenches,
            and rooms without pits have adjacent rooms with no breezys. 
            e.g's in CNF: wumpus11 v ¬stenchXY, pit11 v ¬breezyXY '''
            # can get the left and right
            if X > 1 and X < N:
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X+1)+str(Y)+")")
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X-1)+str(Y)+")")
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X+1)+str(Y)+")")
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X-1)+str(Y)+")")
                print("(or wumpus"+str(X)+str(Y)+" (not stench"+str(X+1)+str(Y)+"))")
                print("(or wumpus"+str(X)+str(Y)+" (not stench"+str(X-1)+str(Y)+"))")
                print("(or pit"+str(X)+str(Y)+" (not breezy"+str(X+1)+str(Y)+"))")
                print("(or pit"+str(X)+str(Y)+" (not breezy"+str(X-1)+str(Y)+"))")
            # can only get the left
            elif X > 1:
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X-1)+str(Y)+")")
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X-1)+str(Y)+")")
                print("(or wumpus"+str(X)+str(Y)+" (not stench"+str(X-1)+str(Y)+"))")
                print("(or pit"+str(X)+str(Y)+" (not breezy"+str(X-1)+str(Y)+"))")
            # can only get the right
            elif X < N:
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X+1)+str(Y)+")")
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X+1)+str(Y)+")")
                print("(or wumpus"+str(X)+str(Y)+" (not stench"+str(X+1)+str(Y)+"))")
                print("(or pit"+str(X)+str(Y)+" (not breezy"+str(X+1)+str(Y)+"))")
            # can get the top and bottom
            if Y > 1 and X < N:
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X)+str(Y+1)+")")
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X)+str(Y-1)+")")
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X)+str(Y+1)+")")
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X)+str(Y-1)+")")
                print("(or wumpus"+str(X)+str(Y)+" (not stench"+str(X)+str(Y+1)+"))")
                print("(or wumpus"+str(X)+str(Y)+" (not stench"+str(X)+str(Y-1)+"))")
                print("(or pit"+str(X)+str(Y)+" (not breezy"+str(X)+str(Y+1)+"))")
                print("(or pit"+str(X)+str(Y)+" (not breezy"+str(X)+str(Y-1)+"))")
            # can get the bottom
            elif Y > 1:
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X)+str(Y-1)+")")
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X)+str(Y-1)+")")
                print("(or wumpus"+str(X)+str(Y)+" (not stench"+str(X)+str(Y-1)+"))")
                print("(or pit"+str(X)+str(Y)+" (not breezy"+str(X)+str(Y-1)+"))")
            # can get the top
            elif Y < N:
                print("(or (not wumpus"+str(X)+str(Y)+") stench"+str(X)+str(Y+1)+")")
                print("(or (not pit"+str(X)+str(Y)+") breezy"+str(X)+str(Y+1)+")")
                print("(or wumpus"+str(X)+str(Y)+" (not stench"+str(X)+str(Y+1)+"))")
                print("(or pit"+str(X)+str(Y)+" (not breezy"+str(X)+str(Y+1)+"))")

            ''' there is no wumpus or pit in any visited room
            e.g. visited11 -> (¬wumpus11 ^ ¬pit11)
            In CNF: ¬visited11 v ¬wumpus11, ¬visited11 v ¬pit11 '''
            print("(or (not visited"+str(X)+str(Y)+") (not wumpus"+str(X)+str(Y)+"))")
            print("(or (not visited"+str(X)+str(Y)+") (not pit"+str(X)+str(Y)+"))")

    wumpusOneSpot += ")"
    print(wumpusOneSpot)

    # rules for this specific instance of the wumpus world
    with open("wumpusInstance.in", "r") as wi:
        # rows specifiying which spots are visited
        for i in range(N):
            line = wi.readline()
            for j in range(N):
                if line[j] == "v":
                    # coordinate system in this loop is a little flipped from wumpus coordinate system
                    print("(or visited"+str(j+1)+str(N-i)+")")
                else:
                    print("(or (not visited"+str(j+1)+str(N-i)+"))")
        
        wi.readline()

        # rows specifying breeze and stench locations
        for i in range(N):
            line = wi.readline()
            for j in range(N):
                if line[j] == "B":
                    print("(or breezy"+str(j+1)+str(N-i)+")")
                    print("(or (not stench"+str(j+1)+str(N-i)+"))")
                elif line[j] == "S":
                    print("(or stench"+str(j+1)+str(N-i)+")")
                    print("(or (not breezy"+str(j+1)+str(N-i)+"))")
                else:
                    print("(or (not stench"+str(j+1)+str(N-i)+"))")
                    print("(or (not breezy"+str(j+1)+str(N-i)+"))")