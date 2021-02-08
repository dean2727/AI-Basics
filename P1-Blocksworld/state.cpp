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

// Print out the state in the horizontal format
void State::print() {
    
}

// Output whether 2 states are equal, for goal-testing
bool State::match(State* state) {

}

// Generate a “key” unique to each state for tracking visited nodes
string State::hash() {

}

// Generate all children of this state given all legal moves
vector<State*> successors() {

}

#endif