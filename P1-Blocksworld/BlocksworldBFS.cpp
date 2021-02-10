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
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#define MAX_ITERS 100000

/********************************
 * function tokenize():
 * Helper function for splitting file input by a delimator
 * Arguments:
 * 1: the parent node
 * *****************************/
void tokenize(string const &str, const char delim, vector<int> &out)
{
    size_t start;
    size_t end = 0;
 
    while ((start = str.find_first_not_of(delim, end)) != string::npos)
    {
        end = str.find(delim, start);
        out.push_back(stoi(str.substr(start, end - start)));
    }
}

/********************************
 * function expand():
 * Return the child nodes of a given node in the state
 * space tree
 * Arguments:
 * 1: the parent node
 * *****************************/
// vector<Node*> expand(Node* parent) {
//     vector<Node*> childNodes;

//     return childNodes;
// }

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
Node* breadthFirstSearch(Node* initNode, State* finalState) {
    if (initNode -> goalTest(finalState)) {
        return initNode;
    }

    queue<Node*> frontier;
    frontier.push(initNode);

    // Track the node states in a hash table
    unordered_map<string, int> reached;
    reached[initNode -> state -> hash()] = 1;
    
    int i = 0;
    Node* currentNode;
    while (!frontier.empty() && i < MAX_ITERS) {
        currentNode = frontier.front();
        frontier.pop();

        vector<Node*> successors = currentNode -> successors();
        for (int i = 0; i < successors.size(); i++) {
            State* childState = successors[i] -> state;

            if (childState -> match(finalState)) {
                return successors[i];
            }

            // if the node has not been reached yet, add to frontier
            if (reached.find(childState -> hash()) == reached.end()) {
                reached[childState -> hash()] = 1;
                frontier.push(successors[i]);
            }
        }

        i++;
    }

    // failure indicated by returning initial node
    return initNode;
}


/********************************
 * Command-line args:
 * 1: A .bwp file representing the initial and final states to be constructed
 ********************************/
int main(int argc, char* argv[]) {
    char* stateFile = argv[1];

    ifstream ifs(stateFile);

    // get first line: S (number of stacks), B (number of blocks), N (number of scrambling steps/moves)
    int S = 0, B = 0, N = 0;
    string line;
    getline(ifs, line);
    vector<int> firstLineTokens;
    const char delim = ' ';
    tokenize(line, delim, firstLineTokens);
    S = firstLineTokens[0];
    B = firstLineTokens[1];
    N = firstLineTokens[2];
    
    vector<vector<char> > initStateStacks;
    vector<vector<char> > finalStateStacks;

    // continue reading the file, getting the initial and final state of the problem
    getline(ifs, line);  // >>>>>>>>>>
    for (int i = 0; i < S; i++) {
        getline(ifs, line);
        vector<char> stack;

        if (line != "") {
            for (int j = 0; j < line.length(); j++) {
                stack.push_back(line[j]);
            }
        }
        initStateStacks.push_back(stack);
    }
    getline(ifs, line);  // >>>>>>>>>>
    for (int i = 0; i < S; i++) {
        getline(ifs, line);
        vector<char> stack;

        if (line != "") {
            for (int j = 0; j < line.length(); j++) {
                stack.push_back(line[j]);
            }
        }
        finalStateStacks.push_back(stack);
    }

    // cout << "DEBUG: initial state:\n" << initState << "\nfinal state:\n" << finalState << endl;

    // initialize new states
    State initState(initStateStacks);
    State finalState(finalStateStacks);
    
    // initialize the starting node in the search space with the initial state and null for parent
    Node startNode(nullptr, &initState);

    // start the search
    Node* goalNode = breadthFirstSearch(&startNode, &finalState);

    // print out the path
}