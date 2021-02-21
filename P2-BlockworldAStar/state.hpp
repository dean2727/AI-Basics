#ifndef STATE_HPP 
#define STATE_HPP

/********************************
 * File name: state.hpp
 * Description: State class. Represents a configuration
 * of blocks
 * Name: Dean Orenstein
 * Class: CSCE 420 500
 * Date: 03/05/2021
 * *****************************/

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class State {
public:
    State(const vector<vector<char> > stacks);
    ~State();

    void printHorizontal();
    void printVertical(int move);
    bool match(State* state);
    string hash();
    vector<State*> successors();
    float heuristic(State& goal);

private:
    vector<vector<char> > blockConfig;
};


#include "state.cpp"
#endif