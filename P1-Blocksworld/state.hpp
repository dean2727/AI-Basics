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

class State {
public:
    void print();
    bool match(State* state);
    string hash();
    vector<State*> successors();
private:
};


#endif