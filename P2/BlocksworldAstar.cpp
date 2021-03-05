/********************************
 * File name: BlockworldAstar.cpp
 * Description: Blockworld AI problem using the A* heuristic in best-first search algorithm
 * to generate a sequence of actions to transform an initial state of blocks
 * into a goal state
 * Name: Dean Orenstein
 * Class: CSCE 420 500
 * Date: 03/05/2021
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
//#define MAX_ITERS 30

// comparator struct used by the priority queue in the algorithm, sorting by f(n)=g(n)+h(n)
struct comparef {
    bool operator() (Node* n1, Node* n2) {
        return n1 -> fn > n2 -> fn;
    }
};

/********************************
 * function tokenize():
 * Helper function for splitting file input by a delimator
 * Arguments:
 * 1: the string to tokenize
 * 2: the delimitor to tokenize by
 * 3: the vector to store the resulting tokens
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
 * function bestFirstSearchAStar() (best-first search in pg. 73 of textbook):
 * Given a search problem for AI, perform an A* best-first search on the state space
 * until the goal(s) state(s) is found. The priority queue is sorted by 
 * f(n)=g(n)+h(n), where h(n) is our heurstic function (this project tests different h(n))
 * Arguments:
 * 1: the initial node (root) in the search
 * 2: the goal state that we desire
 * *****************************/
Node* bestFirstSearchAStar(Node* initNode, State* finalState) {
    priority_queue<Node*, vector<Node*>, comparef> frontier;
    frontier.push(initNode);
    unsigned int maxFrontierSize = frontier.size();

    // Track the node states in a hash table
    unordered_map<State*, Node*> reached;
    reached[initNode -> state] = initNode;
    
    int iters = 2;
    int depth;
    Node* currentNode;
    while (!frontier.empty() && iters < MAX_ITERS) {
        currentNode = frontier.top();
        //cout << "DEBUG: front of queue is node with f(n) = " << currentNode -> fn << endl;
        // cout << "DEBUG: top of queue is:\n";
        // cout << currentNode -> state -> printHorizontal();
        frontier.pop();

        if (currentNode -> goalTest(finalState)) {
            depth = currentNode -> getDepth();
            cout << "success! iter=" << iters << ", ";
            cout << "cost=" << depth << ", ";
            cout << "depth=" << depth << ", ";
            cout << "max queue size=" << maxFrontierSize << "\n";
            return currentNode;
        }

        vector<Node*> successors = currentNode -> successors();
        Node* child;
        State* childState;
        for (unsigned int i = 0; i < successors.size(); i++) {

            // iterations exceeded -> return failure
            if (iters >= MAX_ITERS) {
                return initNode;
            }

            child = successors[i];
            childState = child -> state;

            /* if the node has not been reached yet or the path cost to the child is less than the cost of 
              a different path to the same child, update the reached path and add the node to the frontier (pruning) */
            if (reached.count(childState) == 0 ||
                child -> getDepth() < reached[childState] -> getDepth()) {
                reached[childState] = child;
                //cout << "DEBUG: pushing node to queue with f(n) = " << child -> fn << endl;
                //child -> state -> printHorizontal();
                frontier.push(child);

                iters++;

                // print status every 1000 iterations
                if (iters % 1000 == 0) {
                    cout << "iter=" << iters << ", ";
                    cout << "agenda size=" << frontier.size() << ", ";
                    cout << "curr depth=" << child -> getDepth() << "\n";
                }
            }
        }

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

    // testing the heuristic
    // cout << "DEBUG: Testing heuristic between initial and goal states:\n";
    // cout << "Initial state:\n";
    // initState -> printHorizontal();
    // cout << "Final state:\n";
    // finalState -> printHorizontal();
    // cout << "h(n) for initial state:\n";
    // int h = initState -> heuristic(finalState);
    // cout << "h(n) = " << h << endl;
    
    // initialize the starting node in the search space with the initial state and null for parent
    Node* startNode = new Node(nullptr, initState, finalState);

    // start the search
    Node* goalNode = bestFirstSearchAStar(startNode, finalState);

    // if the goal node is just initial node, couldnt find goal state
    if (&startNode == &goalNode) {
        cout << "failure! could not find goal state\n";
    }
    
    // else, print out the path taken
    else {
        goalNode -> printPath();
    }
}