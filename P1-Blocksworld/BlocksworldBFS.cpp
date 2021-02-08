/********************************
 * File name: BlockworldBFS.cpp
 * Description: Blockworld AI problem using breadth-first search algorithm
 * to generate a sequence of actions to transform an initial state of blocks
 * into a goal state
 * Name: Dean Orenstein
 * Class: CSCE 420 500
 * Date: 02/15/2021
 * *****************************/

//#include "node.hpp"
//#include "state.hpp"
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
// Node* breadthFirstSearch(Node* currentNode, State* finalState) {
//     if (currentNode -> goalTest(finalState)) {
//         return currentNode;
//     }

//     queue<Node*> frontier;
//     frontier.push(currentNode);

//     // Track the node states in a hash table
//     unordered_map<string, int> reached;
//     reached.insert(currentNode -> state -> hash());
    
//     int i = 0;
//     while (!frontier.empty() && i < MAX_ITERS) {
//         currentNode = frontier.pop();

//         for (Node* child : expand(currentNode)) {
//             State* childState = child -> state;

//             if (childState -> match(finalState)) {
//                 return child;
//             }

//             if (reached.find(childState -> hash()) == reached.end()) {
//                 reached.insert(childState -> hash());
//                 frontier.push(child);
//             }
//         }
//         i++;
//     }

//     return failure;
// }


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

    // continue reading the file, getting the initial and final state of the problem
    getline(ifs, line);  // >>>>>>>>>>
    string initState = "", finalState = "";
    for (int i = 0; i < S; i++) {
        getline(ifs, line);
        if (line == "") {
            initState += "-;";
        }
        else {
            initState += line + ";";
        }
    }
    getline(ifs, line);  // >>>>>>>>>>
    for (int i = 0; i < S; i++) {
        getline(ifs, line);
        if (line == "") {
            finalState += "-;";
        }
        else {
            finalState += line + ";";
        }
    }

    cout << "DEBUG: initial state:\n" << initState << "\nfinal state:\n" << finalState << endl;

}