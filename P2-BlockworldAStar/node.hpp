#ifndef NODE_HPP 
#define NODE_HPP

/********************************
 * File name: node.hpp
 * Description: Node class. Represents a state in the
 * state space as well as the path to get to this state
 * Name: Dean Orenstein
 * Class: CSCE 420 500
 * Date: 03/05/2021
 * *****************************/

#include "state.hpp"
#include <vector>
#include <string>
#include <stack>

class Node {
public:
    State* state;
    Node* parent;
    // f(n) score
    int fn;
    // h(n) score (heurstic)
    int hn;

    bool goalTest(State* otherState);
    vector<Node*> successors();
    string hash();
    int printPath();
    int getDepth();

    Node(Node* p, State* s, State* g);
    ~Node();
private:
    State* goal;
};

#include "node.cpp"
#endif