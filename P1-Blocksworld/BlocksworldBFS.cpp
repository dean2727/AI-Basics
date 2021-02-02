/********************************
 * File name: BlockworldBFS.cpp
 * Description: Blockworld AI problem using breadth-first search algorithm
 * to generate a sequence of actions to transform an initial state of blocks
 * into a goal state
 * Name: Dean Orenstein
 * Class: CSCE 420 500
 * Date: 02/15/2021
 * *****************************/

#include "node.hpp"
#include "state.hpp"
#include <unordered_map>
#include <queue>
#include <iostream>
using namespace std;

#define MAX_ITERS 100000


/********************************
 * function expand():
 * Return the child nodes of a given node in the state
 * space tree
 * Arguments:
 * 1: the parent node
 * *****************************/
vector<Node*> expand(Node* parent) {
    vector<Node*> childNodes;

    return childNodes;
}

/********************************
 * function BreadthFirstSearch() (pg. 77 in textbook):
 * Given a search problem for AI, perform a BFS on the state space
 * until the goal(s) state(s) is found.
 * Arguments:
 * 1: the initial node (root) in the search
 * 2: the goal state that we desire
 * Time complexity: O(b^d) (where b = branching factor, d = depth)
 * Space complexity: O(b^d)
 * *****************************/
Node* breadthFirstSearch(Node* currentNode, State* finalState) {
    if (currentNode -> goalTest(finalState)) {
        return currentNode;
    }

    queue<Node*> frontier;
    frontier.push(currentNode);

    // Track the node states in a hash table
    unordered_map<string, int> reached;
    reached.insert(currentNode -> state -> hash());
    
    while (!frontier.empty()) {
        currentNode = frontier.pop();

        for (Node* child : expand(currentNode)) {
            State* childState = child -> state;

            if (childState -> match(finalState)) {
                return child;
            }

            if (reached.find(childState -> hash()) == reached.end()) {
                reached.insert(childState -> hash());
                frontier.push(child);
            }
        }
    }

    return failure;
}


/********************************
 * Command-line args:
 * 1: A .bwp file representing the initial and final states to be constructed
 ********************************/
int main(int argc, char* argv[]) {
    char* stateFile = argv[1];
    cout << stateFile;
}