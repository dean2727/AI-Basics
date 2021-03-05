#ifndef NODE_CPP 
#define NODE_CPP

/********************************
 * File name: node.cpp
 * Description: Node class definition
 * Name: Dean Orenstein
 * Class: CSCE 420 500
 * Date: 03/05/2021
 * *****************************/


#include "node.hpp"
using namespace std;

// constructor. calls State->heurstic(goal) to get the f(n) score
Node::Node(Node* p, State* s, State* g) {
    parent = p;
    state = s;
    goal = g;
    hn = state -> heuristic(goal);

    // if it is our initial node, f(n) is simply the distance calculated from the heuristic
    if (parent == nullptr) {
        fn = 0;
    }

    // else, also include g(n), which is the number of states leading up to the state for this node
    else {
        fn = getDepth() + hn;  // g(n) + h(n)
    } 
}

Node::~Node() {
    delete state;
    delete this;
}

// Check whether this state matches another (like the goal)
bool Node::goalTest(State* otherState) {
    return state -> match(otherState);
}

// gets the successors of the state, and wraps them in Nodes
vector<Node*> Node::successors() {
    unsigned int i;
    vector<State*> successorStates = state -> successors();
    vector<Node*> successorNodes;
    Node* successorNode;

    for (i = 0; i < successorStates.size(); i++) {
        successorNode = new Node(this, successorStates[i], goal);
        successorNodes.push_back(successorNode);
    }

    return successorNodes;
}

// Return hash key of the node's state
string Node::hash() {
    return state -> hash();
}

// Print sequence of states from root down to this
int Node::printPath() {
    stack<Node*> path;
    path.push(this);
    Node* currNode = this;
    
    // obtain the states of the parents all the way up to the very first parent
    while (currNode -> parent != nullptr) {
        currNode = currNode -> parent;
        path.push(currNode);
    }

    // pop off from the stack, printing out the associated states
    int move = 0;
    while (!path.empty()) {
        currNode = path.top();
        cout << "move " << move << ", pathcost=" << currNode -> getDepth() <<
        ", heuristic=" << currNode -> hn << ", f(n)=g(n)+h(n)=" << currNode -> fn << "\n";
        path.top() -> state -> printVertical(move);
        cout << ">>>>>>>>>>\n";

        path.pop();
        move++;
    }

    return 0;
}

int Node::getDepth() {
    int depth = 0;
    Node* currNode = this;
    
    // obtain the states of the parents all the way up to the very first parent
    while (currNode -> parent != nullptr) {
        currNode = currNode -> parent;
        depth += 1;
    }

    return depth;
}

#endif