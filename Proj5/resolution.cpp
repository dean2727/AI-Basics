/********************************
 * File name: resolution.cpp
 * Description: Implements the resolution refutation algorithm for 
 * determining if a sentence is entailed by a KB. Includes a ResPair
 * class, which identifies a pair of clauses during each step of 
 * the procedure, as well as some helper functions. The algorithm
 * uses a minimum clause length heuristic, a generalization of the 
 * unit-clause heuristic, which will give priority to pairs
 * of clauses wherein 1 of them is of least length
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

set<string> KBclauses;

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
        if (parts2[i] == "not" && posLits.count(parts2[i+1])) {
            ret.push_back(parts2[i+1]);
        }
        else if (parts2[i] != "(" && parts2[i] != ")" && parts2[i-1] != "not" && negLits.count(parts2[i])) {
            ret.push_back(parts2[i]);
        }
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
        if (parts2[i] == "not" && posLits.count(parts2[i+1])) {
            return true;
        }
        else if (parts2[i] != "(" && parts2[i] != ")" && parts2[i-1] != "not" && negLits.count(parts2[i])) {
            return true;
        }
    }

    return false;
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
    vector<string> parts1 = tokenize(clause1->toString());
    vector<string> parts2 = tokenize(clause2->toString());
    set<string> seenProps;
    string newClause = "(or ";

    unsigned int i;
    for (i = 2; i < parts1.size()-1; i++) {
        // if this is a "(" and the literal in this "(not <lit>)" is Prop, dont append these parts to newClause
        if (parts1[i] == "(") {
            if (parts1[i+1] == "not" && (parts1[i+2] == Prop)) {
                i += 3;
            }
            else
                newClause += "(";
        }
        else if (parts1[i] == ")" || parts1[i] == "not")
            newClause += parts1[i] + " ";
        // otherwise, just see if this part (that is a potential positive literal) isnt Prop
        else {
            if (parts1[i] != Prop)
                newClause += parts1[i] + " ";
        }
    }
    
    for (i = 2; i < parts2.size()-1; i++) {
        if (parts2[i] == "(") {
            if (parts2[i+1] == "not" && (parts2[i+2] == Prop)) {
                i += 3;
            }
            else
                newClause += "(";
        }
        else if (parts2[i] == ")" || parts2[i] == "not")
            newClause += parts2[i] + " ";
        else {
            if (parts2[i] != Prop)
                newClause += parts2[i] + " ";
        }
    }

    newClause += ")";
    return parse(newClause);
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

    // invalid structure, return false
    if (parts[0] != "(" || parts[1] != "or" || parts[parts.size()-1] != ")")
        return false;

    // check the literals, if there are any
    set<string> seenLits;
    int numOpenParen = 0, numCloseParen = 0;
    for (unsigned int i = 2; i < parts.size()-1; i++) {
        if (parts[i] == "(")
            numOpenParen++;
        else if (parts[i] == ")")
            numCloseParen++;
        else if (parts[i] == "not") {
            // positive and negative literal both in same clause
            if (seenLits.count(parts[i+1]))
                return false;
            seenLits.insert(parts[i+1]);
        }
        else {
            if (parts[i-1] != "not" && seenLits.count(parts[i]))
                return false;
            seenLits.insert(parts[i]);
        }
    }

    if (numOpenParen != numCloseParen)
        return false;
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
    // negate the query and add it to the KB
    Expr* negQuery = negate_query(query);
    KB.push_back(negQuery);
    KBclauses.insert(negQuery->toString());

    // validate the clauses
    unsigned int i, j;
    for (i = 0; i < KB.size(); i++) {
        if (!validateClause(KB[i])) {
            cout << "error! at least one of the clauses in the KB is not in the proper format!!" << endl;
            return 0;
        }
    }

    // show the knowledge base
    show_kb(KB, KBclauses);
    cout << endl;
    int nextSpot = KB.size();

    // priority queue for the MCL heuristic
    priority_queue<ResPair, vector<ResPair>, decltype(&cmpRes)> newRes(cmpRes);

    // push all resolvable pairs from the KB
    for (i = 0; i < KB.size(); i++) {
        for (j = i+1; j < KB.size(); j++) {
            if (resolvable(KB[i], KB[j])) {
                // get min # of literals between the 2 clauses, create ResPair object
                int len = min(KB[i]->sub.size(), KB[j]->sub.size());
                ResPair rp(i, j, len);
                newRes.push(rp);
            }
        }
    }

    // go until just 2 remaining clauses, and if they cause a contradiction, return true
    int iter = 0;
    while (!newRes.empty() && iter < MAX_ITERS) {
        //if (iter % 1000 == 0) cout << iter << endl;
        ResPair rp = newRes.top();
        newRes.pop();
        Expr* Ci = KB[rp.i];
        Expr* Cj = KB[rp.j];
        cout << "iteration=" << iter << ", clauses=" << nextSpot-1 << "\n";
        cout << "resolving clauses " << rp.i << " and " << rp.j << ": " << Ci->toString() << " , " << Cj->toString() << "\n";

        // let Props be a list of props that appear in both clauses as opposite literals
        vector<string> props = matchingPropositions(Ci, Cj);
        unsigned int P;
        for (P = 0; P < props.size(); P++) {
            Expr* resolvent = resolve(Ci, Cj, props[P]);
            cout << "resolvent = " << resolvent->toString() << "\n";
            if (resolvent -> toString() == "(or)") {
                return true;
            }
            if (validateClause(resolvent) == true && !KBclauses.count(resolvent->toString())) {
                // see if the resolvant is resolvable with any other clause in the KB            
                for (i = 0; i < KB.size(); i++) {
                    if (resolvable(KB[i], resolvent)) {
                        int len = min(KB[i]->sub.size(), resolvent->sub.size());
                        ResPair rp(i, nextSpot, len);
                        newRes.push(rp);
                    }
                }
                
                cout << nextSpot << ". " << resolvent->toString() << "\n\n";
                KB.push_back(resolvent);
                KBclauses.insert(resolvent->toString());
                nextSpot++;
            }
            else {
                cout << "resolvent is invalid or is already in the KB!!\n\n";
            }
        }
        iter++;
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

        // Testing suite
        //vector<Expr*> KB = load_kb("test.kb");
        // cout << "Testing resolvable():" << endl;
        // cout << "args: " << KB[0]->toString() << ", " << KB[1]->toString() << endl;
        // assert(resolvable(KB[0], KB[1]) == true);
        // cout << endl;

        // cout << "Testing matchingPropositions():" << endl;
        // cout << "args: " << KB[0]->toString() << ", " << KB[1]->toString() << endl;
        // vector<string> mp = matchingPropositions(KB[0], KB[1]);
        // cout << mp.size() << endl << endl;

        // cout << "Testing resolve():" << endl;
        // cout << "args: " << KB[0]->toString() << ", " << KB[1]->toString() << ", " << mp[0] << endl;
        // cout << resolve(KB[0], KB[1], mp[0])->toString() << endl << endl;

        // cout << "Testing validateClause():" << endl;
        // cout << "arg: " << KB[0]->toString() << endl;
        // assert(validateClause(KB[0]) == true);
        // cout << "arg: " << KB[1]->toString() << endl;
        // assert(validateClause(KB[1]) == true);
        // // repeating literals
        // cout << "arg: " << KB[2]->toString() << endl;
        // assert(validateClause(KB[2]) == false);
        // cout << endl;
    }
    catch (runtime_error& e) {
        cout << e.what() << endl;
    }
}