/********************************
 * File name: BlockworldBFS.cpp
 * Description: Blockworld AI problem using breadth-first search algorithm
 * to generate a sequence of actions to transform an initial state of blocks
 * into a goal state
 * Name: Dean Orenstein
 * Class: CSCE 420 500
 * Date: 02/15/2021
 * *****************************/

#include "state.hpp"
#include "node.hpp"
#include <unordered_map>
#include <queue>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
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
    unsigned int maxFrontierSize = frontier.size();

    // Track the node states in a hash table
    unordered_map<string, int> reached;
    reached[initNode -> state -> hash()] = 1;
    
    int iters = 2;
    Node* currentNode;
    while (!frontier.empty() && iters < MAX_ITERS) {
        currentNode = frontier.front();

        vector<Node*> successors = currentNode -> successors();
        for (unsigned int i = 0; i < successors.size(); i++) {

            // iterations exceeded -> return failure
            if (iters >= MAX_ITERS) {
                return initNode;
            }

            State* childState = successors[i] -> state;
            if (childState -> match(finalState)) {
                cout << "success! iter=" << iters << ", ";
                cout << "depth=" << successors[i] -> getDepth() << ", ";
                cout << "max queue size=" << maxFrontierSize << "\n";
                return successors[i];
            }

            // if the node has not been reached yet, add to frontier
            if (reached.find(childState -> hash()) == reached.end()) {
                reached[childState -> hash()] = 1;
                frontier.push(successors[i]);
                iters++;

                // print status every 1000 iterations
                if (iters % 1000 == 0) {
                    cout << "iter=" << iters << ", ";
                    cout << "agenda size=" << frontier.size() << ", ";
                    cout << "curr depth=" << successors[i] -> getDepth() << "\n";
                }
            }
        }

        frontier.pop();
        if (frontier.size() > maxFrontierSize) {
            maxFrontierSize = frontier.size();
        }
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
            for (unsigned int j = 0; j < line.length(); j++) {
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

    // initialize new states
    State* initState = new State(initStateStacks);
    State* finalState = new State(finalStateStacks);
    
    // initialize the starting node in the search space with the initial state and null for parent
    Node* startNode = new Node(nullptr, initState);

    // start the search
    Node* goalNode = breadthFirstSearch(startNode, finalState);

    // if the goal node is just initial node, couldnt find goal state
    if (&startNode == &goalNode) {
        cout << "failure! could not find goal state\n";
    }
    
    // else, print out the path taken
    else {
        goalNode -> printPath();
    }
}