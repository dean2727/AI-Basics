#ifndef STATE_HPP 
#define STATE_HPP

/********************************
 * File name: state.hpp
 * Description: State class. Represents a configuration
 * of blocks
 * Name: Dean Orenstein
 * Class: CSCE 420 500
 * Date: 02/15/2021
 * *****************************/

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class State {
public:
    State(const vector<vector<char> > stacks);
    void print();
    bool match(State* state);
    string hash();
    vector<State*> successors();
private:
    vector<vector<char> > blockConfig;
};


#include "state.cpp"
#endif