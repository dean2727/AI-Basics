/********************************
 * File name: resolution.cpp
 * Description: Implements the resolution refutation algorithm for 
 * determining if a sentence is entailed by a KB. Includes a ResPair
 * class, which identifies a pair of clauses during each step of 
 * the procedure, as well as some helper functions. The algorithm
 * uses a minimum clause length heuristic, a generalization of the 
 * unit-clause heuristic, which will take give priority to pairs
 * of clauses that have the shortest total length and where 1 of the
 * clauses is a unit clause 
 * Name: Dean Orenstein
 * Class: CSCE 420 500
 * Date: 04/28/2021
 * *****************************/

#include "parser.hpp"
#include "parser.cpp"
#include <queue>
#include <set>
using namespace std;

#define MAX_ITERS 10000

set<Expr*> KBclauses;

class ResPair {
public:
    int i, j;
    int score;  // aggregated length of the 2 clauses
    ResPair(int a, int b, int sc) { i=a; j=b; score=sc; }
};

// compare pairs by increasing score
bool cmpRes(const ResPair& rp1, const ResPair& rp2) {
    return rp1.score > rp2.score;
}

/********************************
 * function matchingPropositions():
 * Helper function that returns a list of all proposition
 * symbols (as strings) that appear as a positive literal
 * on one of the clauses and a negative literal in the other
 * Arguments:
 * 1: clause 1
 * 2: clause 2
 * *****************************/
vector<string> matchingPropositions(Expr* clause1, Expr* clause2) {
    vector<string> ret;
    set<string> posLits;
    set<string> negLits;
    vector<string> parts1 = tokenize(clause1->toString());
    vector<string> parts2 = tokenize(clause2->toString());

    for (unsigned int i = 2; i < parts1.size()-1; i++) {
        if (parts1[i] == "not")
            negLits.insert(parts1[i+1]);
        else if (parts1[i] != "(" && parts1[i] != ")" && parts1[i-1] != "not")
            posLits.insert(parts1[i]);
    }
    for (unsigned int i = 2; i < parts2.size()-1; i++) {
        if (parts2[i] == "not" && posLits.count(parts2[i+1]))
            ret.push_back(parts2[i+1]);
        else if (parts2[i] != "(" && parts2[i] != ")" && parts2[i-1] != "not" && negLits.count(parts2[i]))
            ret.push_back(parts2[i]);
    }

    // remove any duplicates
    unique(ret.begin(), ret.end());
    return ret;
}

/********************************
 * function resolvable():
 * Helper function for seeing if 2 clauses share a common
 * literal but with opposite signs
 * Arguments:
 * 1: clause 1
 * 2: clause 2
 * *****************************/
bool resolvable(Expr* clause1, Expr* clause2) {
    vector<string> common = matchingPropositions(clause1, clause2);
    if (common.empty()) return false;
    else return true;
}

/********************************
 * function resolve():
 * Helper function that cancels out instances of “Prop” or “(not Prop)”,
 * collects the remaining literals between the two clauses, and makes a
 * new disjunctive clause out of them. Also removes repeated literals
 * from the resolvent
 * Arguments:
 * 1: clause 1
 * 2: clause 2
 * 3: "Prop"
 * *****************************/
Expr* resolve(Expr* clause1, Expr* clause2, string Prop) {

}

/********************************
 * function validateClause():
 * Helper function that validates the form of a clause and ensures 
 * that a positive and negative version of the same literal does
 * not appear in it
 * Arguments:
 * 1: clause
 * *****************************/
bool validateClause(Expr* clause) {
    vector<string> parts = tokenize(clause->toString());
    if (parts[0] != "(" || parts[1] != "or" || parts[-1] != ")")
        return false;

    // check the literals, if there are any
    set<string> seenLits;
    int numOpenParen = 0, numCloseParen = 0;
    for (unsigned int i = 2; i < parts.size()-1; i++) {
        if (parts[i] == "(") numOpenParen++;
        else if (parts[i] == ")") numCloseParen++;
        else if (parts[i] == "not") {
            // positive and negative literal both in same clause
            if (seenLits.count(parts[i+1])) return false;
        }
        else {
            if (parts[i-1] != "not") seenLits.insert(parts[i]);
        }
    }

    if (numOpenParen != numCloseParen) return false;
    return true;
}

/********************************
 * function resolution():
 * The resolution refutation procedure. Given a KB and a negated query
 * of the form "(or <literal>)"
 * not appear in it
 * Arguments:
 * 1: clause
 * *****************************/
bool resolution(vector<Expr*>& KB, Expr* query) {
    // validate the clauses
    unsigned int i, j;
    for (i = 0; i < KB.size(); i++) {
        if (!validateClause(KB[i])) {
            cout << "error! at least one of the clauses in the KB is not in the proper format!!" << endl;
            return 0;
        }
    }

    // show the initial knowledge base
    show_kb(KB, KBclauses);
    cout << endl;
    int nextSpot = KB.size();

    // negate the query and add it to the KB
    Expr* negQuery = negate_query(query);
    KB.push_back(negQuery);
    KBclauses.insert(negQuery);

    // priority queue for the MCL heuristic
    priority_queue<ResPair, vector<ResPair>, decltype(&cmpRes)> newRes(cmpRes);

    // push all resolvable pairs from the KB
    for (i = 0; i < KB.size(); i++) {
        for (j = i+1; j < KB.size(); j++) {
            if (resolvable(KB[i], KB[j])) {
                // get # of literals between the 2 clauses, create ResPair object
                int len = get_MCL(KB[i], KB[j]);
                ResPair rp(i, j, len);
                newRes.push(rp);
            }
        }
    }

    // go until just 2 remaining clauses, and if they cause a contradiction, return true
    int iter = 0;
    while (!newRes.empty() && iter++ < MAX_ITERS) {
        ResPair rp = newRes.top();
        newRes.pop();
        Expr* Ci = KB[rp.i];
        Expr* Cj = KB[rp.j];

        // let Props be a list of props that appear in both clauses as opposite literals
        vector<string> props = get_common_literals();
        unsigned int P;
        for (P = 0; P < props.size(); P++) {
            Expr* resolvent = resolve(Ci, Cj, props[P]);
            if (resolvent -> toString() == "(or )") {
                return true;
            }
            if (validateClause(resolvent) == false || KBclauses.count(resolvent)) {
                continue;
            }
            // see if the resolvant is resolvable with any other clause in the KB            
            for (i = 0; i < KB.size(); i++) {
                if (resolvable(KB[i], resolvent)) {
                    // get # of literals between the 2 clauses
                    int len = get_MCL(KB[i], resolvent);
                    ResPair rp(i, nextSpot, len);
                    newRes.push(rp);
                }
            }
            KB.push_back(resolvent);
            KBclauses.insert(resolvent);
            nextSpot++;
        }
    }
    
    return false;
}


int main(int argc, char* argv[]) {
    try {
        // load the KB and get negated query
        vector<Expr*> KB = load_kb(argv[1]);
        string negQuery = argv[2];

        // run resolution refutation
        if (resolution(KB, parse(negQuery))) {
            cout << "success! derived empty clause, so Q is entailed" << endl;
        }
        else {
            cout << "failure! empty clause could not be derived, so Q is not entailed" << endl;
        }
    }
    catch (runtime_error& e) {
        cout << e.what() << endl;
    }
}