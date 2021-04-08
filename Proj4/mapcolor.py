'''
File name: mapcolor.py
Creates all of the rules in CNF for the knowledge base
of the Australia Map Coloring problem, writing to standard output
(output is meant to be redirected into a .cnf file)
Name: Dean Orenstein
Class: CSCE 420
Date: 04/09/2021
'''

if __name__ == "__main__":
    numStates = 7
    states = ["WA", "NT", "SA", "Q", "NSW", "V", "T"]
    adjStates = {"WA":["NT","SA"], "NT":["WA","SA","Q"], "SA":["WA","NT","Q","NSW","V"],
                "Q":["NT","SA","NSW"], "NSW":["SA","Q","V"], "V":["SA","NSW"]}
    # Keep track of the states we make adjacent state restrictions for, since we dont have duplicate rules
    visitedStates = set()

    # Each state is at least 1 of 3 colors
    for state in states:
        print("(or "+state+"R "+state+"G "+state+"B)")
    
    # Each state must be at most 1 of 3 colors
    # e.g. CNF conversion: WAR -> ¬WAG ^ ¬WAB => ¬WAR v ¬WAG ^ ¬WAB => 2 clauses: ¬WAR v ¬WAG and ¬WAR v ¬WAB
    for state in states:
        print("(or (not "+state+"R) (not "+state+"G))")
        print("(or (not "+state+"R) (not "+state+"B))")
        print("(or (not "+state+"G) (not "+state+"B))")
    
    # Adjacent states must be different colors
    # e.g. CNF conversion: WAR -> ¬NTR ^ ¬SAR => ¬WAR v ¬NTR ^ ¬SAR => 2 clauses: ¬WAR v ¬NTR and ¬WAR v ¬SAR 
    for state in adjStates:
        visitedStates.add(state)
        for adj in adjStates[state]:
            if adj not in visitedStates:
                print("(or (not "+state+"R) (not "+adj+"R))")
                print("(or (not "+state+"G) (not "+adj+"G))")
                print("(or (not "+state+"B) (not "+adj+"B))")
