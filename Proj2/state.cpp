#ifndef STATE_CPP 
#define STATE_CPP

/********************************
 * File name: state.cpp
 * Description: State class definition
 * Name: Dean Orenstein
 * Class: CSCE 420 500
 * Date: 03/05/2021
 * *****************************/
#include "state.hpp"

// constructor
State::State(const vector<vector<char> > stacks) {
    blockConfig = stacks;
}

State::~State() {
    delete this;
}

// Print out the state in the horizontal (slightly more readable) format
void State::printHorizontal() {
    unsigned int i;
    // first, get max stack size in the state
    int maxStackHeight = 0, currHeight;
    for (i = 0; i < blockConfig.size(); i++) {
        currHeight = blockConfig[i].size();
        if (currHeight > maxStackHeight) {
            maxStackHeight = currHeight;
        }
    }

    // print out the contents from the top, down (from the right, left)
    for (int height = maxStackHeight - 1; height >= 0; height--) {
        for (unsigned int stack = 0; stack < blockConfig.size() * 2 - 1; stack++) {
            // printing out a space every other column, so check if stack # is even
            if (stack % 2 == 0) {
                vector<char> thisStack = blockConfig[stack / 2];
                int thisStackSize = 0;
                if (!thisStack.empty()) {
                    thisStackSize = thisStack.size();
                }

                // current height may exceed the number of blocks in the stack
                if (height > thisStackSize - 1) {
                    cout << " ";
                }
                else {
                    cout << thisStack[height];
                }
            }
            else {
                cout << " ";
            }
        }
        cout << "\n";
    }

    for (i = 0; i < blockConfig.size() * 2; i++) {
        cout << "-";
    }
    cout << "\n";
}

// Print out the state in the vertical format
void State::printVertical(int move) {
    unsigned int i, j;
    
    for (i = 0; i < blockConfig.size(); i++) {
        for (j = 0; j < blockConfig[i].size(); j++) {
            cout << blockConfig[i][j];
        }
        cout << "\n";
    }
}

// Output whether 2 states are equal, for goal-testing
bool State::match(State* state) {
    if (this -> hash() == state -> hash()) {
        return true;
    }
    return false;
}

// Generate a “key” unique to each state for tracking visited nodes
string State::hash() {
    unsigned int i;
    string hash = "";

    for (i = 0; i < blockConfig.size(); i++) {
        for (unsigned int j = 0; j < blockConfig[i].size(); j++) {
            hash += blockConfig[i][j];
        }
        hash += ';';
    }

    return hash;
}

// Generate all children of this state given all legal moves
vector<State*> State::successors() {
    unsigned int i;
    vector<State*> newStates;

    // for each stack, take a block off and put it in all other stacks (one new state per put)
    for (i = 0; i < blockConfig.size(); i++) {
        vector<char> thisStack = blockConfig[i];

        // needs to have at least 1 block
        if (!thisStack.empty()) {
            char blockOff = thisStack.back();
            thisStack.pop_back();

            unsigned int count = 1, idx = i + 1;
            while (count != blockConfig.size()) {
                // need to reinitialize the stacksForNewState each time so the previous stack change is reset
                vector<vector<char> > stacksForNewState = blockConfig;
                stacksForNewState[i] = thisStack;  // the stack with the popped block remains common across all new states
                
                // may need to loop back around to the beginning of the stacks vector
                if (idx == blockConfig.size()) {
                    idx = 0;
                }

                // retrieve the correct stack, place the block on it, update the stack, and create new state
                vector<char> stackWithNewBlock = blockConfig[idx];
                stackWithNewBlock.push_back(blockOff);
                stacksForNewState[idx] = stackWithNewBlock;
                State* newState = new State(stacksForNewState);

                newStates.push_back(newState);
                count++;
                idx++;
            }
        }
    }

    return newStates;
}

vector<vector<char> > State::getConfig() {
    return blockConfig;
}


/*
heuristics tried:
1. euclidean distance between each block in the state vs the goal state (0 if block is in the right spot)
2. manhatten distance
3. sum of differences in y (stack height) for each block
4. sum of differences in x (which stack it is) for each block
5. add 2 for every block thats not currently directly on top of the block on which it has to be in the goal state,
or if there is such a block below it
6. if Block A in the goal state is supposed to be on top of Block B and under Block C and in the current state it is
neither on top of B or under C, then add 2 to the heuristic
7. 2x the number of blocks needing to be moved once, which are blocks that sit on blocks different than 
what they sit on in the goal state (or has such a block below it) and 4x the number of blocks needing
to be moved twice, which are blocks that have the correct block below it but must be moved (different stack) 
or have moved-twice blocks somewhere below it
*/

// using h5
int State::heuristic(State* goal) {
    int h = 0;

    vector<vector<char> > goalConfig = goal -> getConfig();
    int thisBlockConfigSize = blockConfig.size(), goalBlockConfigSize = goalConfig.size();
    int thisStackSize, goalStackSize;
    // maps each block to the stack they are on
    map<char, int> stackNums;

    // maps each block in the goal state to the block that should be under it
    map<char, char> underBlocks;
    for (int gbcs = 0; gbcs < goalBlockConfigSize; gbcs++) {  // gcbs = goal block config stack
        goalStackSize = goalConfig[gbcs].size();
        for (int gb = 0; gb < goalStackSize; gb++) {  // gb = goal block
            stackNums.insert(make_pair(goalConfig[gbcs][gb], gbcs));

            // no block under it, just have a placeholder of !
            if (gb == 0) {
                underBlocks.insert(make_pair(goalConfig[gbcs][gb], '!'));
            }
            else {
                underBlocks.insert(make_pair(goalConfig[gbcs][gb], goalConfig[gbcs][gb-1]));
            }
        }
    }

    char thisBlock, blockUnder;

    for (int tbcs = 0; tbcs < thisBlockConfigSize; tbcs++) {  // tbcs = this block config stack
        thisStackSize = blockConfig[tbcs].size();
        bool badUnder = false;
        for (int tb = 0; tb < thisStackSize; tb++) {  // tb = this block
            thisBlock = blockConfig[tbcs][tb];
            if (tb == 0) {
                blockUnder = '!';
            }
            else {
                blockUnder = blockConfig[tbcs][tb - 1];
            }

            if (blockUnder == '!' && stackNums[thisBlock] != tbcs) {
                h += 2;
                badUnder = true;
            }

            // if block under is not equal, add 2
            else if (blockUnder != underBlocks[thisBlock]) {
                h += 2;
                badUnder = true;
            }

            // even if the block under is fine, if we've seen a wrong under block in the pile, add 2
            else if (badUnder) h += 2;            
        }
    }

    return h;
}

#endif