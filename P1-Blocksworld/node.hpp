#ifndef NODE_HPP 
#define NODE_HPP

/********************************
 * File name: node.hpp
 * Description: Node class. Represents a state in the
 * state space as well as the path to get to this state
 * Name: Dean Orenstein
 * Class: CSCE 420 500
 * Date: 02/15/2021
 * *****************************/

#include "state.hpp"
#include <vector>
#include <string>

class Node {
public:
    State* state;

    bool goalTest(State* state);
    vector<Node*> successors();
    string hash();
    int printPath();
private:

};

#endif