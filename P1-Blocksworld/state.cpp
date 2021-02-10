#ifndef STATE_CPP 
#define STATE_CPP

/********************************
 * File name: state.cpp
 * Description: State class definition
 * Name: Dean Orenstein
 * Class: CSCE 420 500
 * Date: 02/15/2021
 * *****************************/

#include "state.hpp"

// constructor
State::State(const vector<vector<char> > stacks) {
    blockConfig = stacks;
}

// Print out the state in the horizontal format
void State::print() {
    // first, get max stack size in the state
    int maxStackHeight = 0, currHeight;
    for (int i = 0; i < blockConfig.size(); i++) {
        currHeight = blockConfig[i].size();
        if (currHeight > maxStackHeight) {
            maxStackHeight = currHeight;
        }
    }

    // print out the contents from the top, down (from the right, left)
    for (int height = maxStackHeight - 1; height >= 0; height--) {
        for (int stack = 0; stack < blockConfig.size() * 2 - 1; stack++) {

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
        cout << endl;
    }

    for (int i = 0; i < blockConfig.size() * 2; i++) {
        cout << "-";
    }
    cout << endl;
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
    string hash = "";

    for (int i = 0; i < blockConfig.size(); i++) {
        for (int j = 0; j < blockConfig[i].size(); j++) {
            hash += blockConfig[i][j];
        }
        hash += ';';
    }

    return hash;
}

// Generate all children of this state given all legal moves
vector<State*> State::successors() {
    vector<State*> newStates;

    // for each stack, take a block off and put it in all other stacks (one new state per put)
    for (int i = 0; i < blockConfig.size(); i++) {
        vector<char> thisStack = blockConfig[i];

        // needs to have at least 1 block
        if (!thisStack.empty()) {
            char blockOff = thisStack.back();
            thisStack.pop_back();

            int count = 1, idx = i + 1;
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
                State newState(stacksForNewState);

                newStates.push_back(&newState);
                count++;
                idx++;
            }
        }
    }

    return newStates;
}

#endif