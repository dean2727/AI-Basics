/***********************************************************************
 * File name: DPLL.cpp
 * Contains the DPLL (boolean satisfiability) algorithm, which does
 * logical inference to see if a particular sentence is entailed by
 * a model. It may or may not use the unit clause heuristic (-unit flag),
 * and it is tested on both the Australia map coloring problem as well as
 * the N-queens problem.
 * Name: Dean Orenstein
 * Class: CSCE 420
 * Date: 04/09/2021
 **********************************************************************/

#include "parser.hpp"
#include "parser.cpp"
#include <unordered_map>
#include <iostream>
using namespace std;

set<string> uniqueSymbols;
int DPLL_CALLS;
int numClauses;
bool UNIT_CLAUSE;

/********************************
 * function printClauses(model):
 * Prints the truth assignments of the symbols for the current
 * model
 *******************************/
void printClauses(MODEL* model, vector<Expr*> clauses) {
    cout << "model: ";
    for (auto& it : uniqueSymbols) {
        cout << it << "=";
        // if the symbol is not in the hash table, then it is not yet assigned a truth value
        if (!model->count(it)) cout << "? ";
        else {
            if (model->at(it) == true) cout << "T ";
            else cout << "F ";
        }
    }
    cout << endl;
    
    int numSat = 0;
    for (unsigned int i = 0; i < clauses.size(); i++) {
        if (evaluate_clause(clauses[i], model) == 1) numSat++;
    }
    cout << "num clauses satisfied: " << numSat << " out of " << numClauses << endl;
}

// Helper function to deep copy a model and either update or insert into it
MODEL* updateModel(MODEL* model, string symbol, bool newVal) {
    MODEL* ret = new MODEL;
    for (auto& x : *model) {
        ret->insert(x);
    }
    if (ret->count(symbol)) ret->at(symbol) = newVal;
    else ret->insert(make_pair(symbol, newVal));
    return ret;
}

/********************************
 * function findUnitClause(clauses, model):
 * The unit clause heuristic for DPLL. A unit clause
 * is a clause where all the literals except one of them
 * are false (resulting in a singular literal, which can be
 * evaluated easily). Returns a pair, representing the symbol
 * to be assigned a truth value and the truth value itself.
 * If the symbol is currently unassigned, then symbol is NULL
 *******************************/
pair<string, bool> findUnitClause(vector<Expr*> clauses, MODEL* model) {
    for (unsigned int i = 0; i < clauses.size(); i++) {
        if (is_unit_clause(clauses[i], model)) {
            return get_unit_clause(clauses[i], model);
        }
    }
    string emptyStr;
    return make_pair(emptyStr, true);
}

/********************************
 * function DPLL(clauses, model) (pg. 234 in textbook):
 * Given a CNF version of a KB, recursively searches the possible
 * truth assignments for the symbols in the model until a complete
 * and consistent model is found. Uses the unit-clause heuristic.
 * If a nullptr is returned, then a model could not be found.
 *******************************/
MODEL* DPLL(vector<Expr*> clauses, set<string> unassignedSymbols, MODEL* model) {
    DPLL_CALLS++;
    printClauses(model, clauses);

    // if every clause in clauses is true in model, then return true (the model)
    // if some clause is false, return false (nullptr)
    bool symbolUnassigned = false;
    int eval;
    for (unsigned int i = 0; i < clauses.size(); i++) {
        eval = evaluate_clause(clauses[i], model);
        //cout << "DEBUG: " << clauses[i]->toString() << " evaluated to " << eval << endl;
        if (eval == 0) {
            cout << "backtracking..." << endl;
            return nullptr;
        }
        else if (eval == -1) {
            symbolUnassigned = true;
        }
    }
    if (!symbolUnassigned) {
        return model;
    } 

    // if we get here, a symbol is unassigned, so find unit clause (if flag is set)
    if (UNIT_CLAUSE) {
        pair<string, bool> uclause = findUnitClause(clauses, model);

        // if there is a unit clause
        if (!uclause.first.empty()) {
            MODEL* ucModel = updateModel(model, uclause.first, uclause.second);
            cout << "forcing " << uclause.first << "=";
            if (uclause.second) cout << "1" << endl << endl;
            else cout << "0" << endl << endl;
            return DPLL(clauses, unassignedSymbols, ucModel);
        }
    }
    
    // otherwise, recurse DPLL with the next unassigned literal being true and false, and take best model
    set<string>::iterator it = unassignedSymbols.begin();
    string lit = *it;
    unassignedSymbols.erase(it);
    MODEL* falseRecurse = updateModel(model, lit, false);
    cout << "trying " << lit << "=0" << endl << endl;
    MODEL* frCall = DPLL(clauses, unassignedSymbols, falseRecurse);
    if (frCall != nullptr) {
        return frCall;
    }
    else {
        MODEL* trueRecurse = updateModel(model, lit, true);
        cout << "trying " << lit << "=1" << endl << endl;
        return DPLL(clauses, unassignedSymbols, trueRecurse);
    }
}

int main(int argc, char* argv[]) {
    try {
        // load the KB
        vector<Expr*> KB = load_kb(argv[1]);
        
        // set the flag (dont use unit clause heuristic if set)
        if (argc == 3) {
            UNIT_CLAUSE = false;
        }
        else {
            UNIT_CLAUSE = true;
        }
        
        // initialize the model (hash table) to be the empty truth assignment over all symbols
        MODEL* model = new MODEL;

        // get the number of clauses
        numClauses = 0;
        for (unsigned int i = 0; i < KB.size(); i++) {
            numClauses++;
        }

        // fill the symbols containers
        uniqueSymbols = get_unique_symbols(KB);
        set<string> unassignedSymbols = uniqueSymbols;

        // run DPLL
        DPLL_CALLS = 0;
        model = DPLL(KB, unassignedSymbols, model);

        // print result
        if (model == nullptr) {
            cout << "unsatisfiable" << endl;
            cout << "number of DPLL calls=" << DPLL_CALLS << endl;
        }
        else {
            cout << "success!" << endl;
            cout << "number of DPLL calls=" << DPLL_CALLS << endl;
            cout << "here is a model:" << endl;
            for (auto& it : uniqueSymbols) {
                cout << it << " = ";
                if (model->at(it) == true) {
                    cout << "T" << endl;
                }
                else {
                    cout << "F" << endl;
                }
            }
        }  
    }
    catch (runtime_error& e) {
        cout << e.what() << endl;
    }
}