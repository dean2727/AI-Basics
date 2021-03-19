/***********************************************************************
 * File name: NatDed.cpp
 * Description: Contains implementations of the rules of inference for
 * the natural deduction to be done for Sammys Sport Shop (in sammys.cpp).
 * Each ROI is a function that returns an expression that is the result
 * of performing the ROI on the parameters
 * Name: Dean Orenstein
 * Class: CSCE 420
 * Date: 03/22/2021
 **********************************************************************/

#include "parser.hpp"
#include "parser.cpp"
#include <iostream>
#include <stack>
using namespace std;

// helper function to extract the sentence A (p = 0) or B (p = 1) from A^B or A->B
string getSentence(const vector<string>& parts, int p) {
    int i, parentheses = 1;
    string sentence = "";
    
    // if p is 0, extract A (loop forwards until parentheses cancel)
    if (p == 0) {
        i = 1;
        sentence += parts[0];

        while (parentheses > 0) {
            //cout << "this part is " << parts[i] << endl;
            sentence += " " + parts[i];
            if (parts[i] == "(") parentheses++;
            if (parts[i] == ")") parentheses--;
            i++;
        }
    }

    // else, p is 1, so extract B (loop backwards until parentheses cancel)
    else {
        stack<string> backwardsPartStack;
        i = parts.size() - 1;
        backwardsPartStack.push(parts[i]);
        i--;
        
        while (parentheses > 0) {
            //cout << "this part is " << parts[i] << endl;
            backwardsPartStack.push(" ");
            backwardsPartStack.push(parts[i]);

            if (parts[i] == "(") parentheses--;
            if (parts[i] == ")") parentheses++;
            i--;
        }

        while (!backwardsPartStack.empty()) {
            sentence += backwardsPartStack.top();
            backwardsPartStack.pop();
        }
    }

    return sentence;
}

// helper function to extract the expression A (clause = 0) or B (clause = 1) from A^B or A->B
Expr* extractExpression(int clause, const vector<string>& partsNeeded) {
    if (clause == 0) {
        // is A an atom?
        if (partsNeeded[0] != "(") {
            return parse(partsNeeded[0]);
        }
    }

    else {
        int pnSize = partsNeeded.size();
        // is B an atom?
        if (partsNeeded[pnSize - 1] != ")") return parse(partsNeeded[pnSize - 1]);
    }

    // else, its a list
    string sentence = getSentence(partsNeeded, clause);
    return parse(sentence);
}

/*********************
 * From this: A->B, A
 * Derive this: B
 ********************/
Expr* ModusPonens(Expr* s1, Expr* s2) {
    Expr* ret = nullptr;
    vector<string> parts = tokenize(s1 -> toString());
    // dont consider the parts "(", "implies", and the last ")"
    vector<string> partsNeeded(parts.begin() + 2, parts.end() - 1);

    // extract the antecedents from s1
    Expr* antecedents = extractExpression(0, partsNeeded);

    // make sure s2 matches the antecedents of s1
    if (Eq(s2, antecedents)) {

        // extract the consequents of s1
        ret = extractExpression(1, partsNeeded);
    }

    return ret;
}

/*********************
 * From this: A->B
 * Derive this: ¬AvB
 ********************/
Expr* ImplicationElimination(Expr* s1) {
    vector<string> parts = tokenize(s1 -> toString());

    // only need the relevant parts for A and B (remove the "(", "implies", and last ")")
    vector<string> partsNeeded(parts.begin() + 2, parts.end() - 1);

    // extract A and negate it
    string A = extractExpression(0, partsNeeded) -> toString();
    string negA = "(not " + A + ")";

    // extract B
    string B = extractExpression(1, partsNeeded) -> toString();

    // return a parsed version of negated A and B or'ed together
    return parse("(or " + negA + B + ")");
}

/*********************
 * From this: A^B
 * Derive this: A
 ********************/
Expr* AndElimination(Expr* s1, int clause) {
    vector<string> parts = tokenize(s1 -> toString());
    // cout << "tokenized!\n";
    // for (int i=0;i<parts.size();i++){
    //     cout << parts[i] << endl;
    // }

    // dont consider the parts "(", "and", and the last ")"
    vector<string> partsNeeded(parts.begin() + 2, parts.end() - 1);
    // for (int i=0;i<partsNeeded.size();i++){
    //     cout << partsNeeded[i] << endl;
    // }

    return extractExpression(clause, partsNeeded);
}

/*********************
 * From this: A, B
 * Derive this: A^B
 ********************/
Expr* AndIntroduction(Expr* s1, Expr* s2) {
    return parse("(and " + s1 -> toString() + " " + s2 -> toString() + ")");
}

/*********************
 * From this: A, B
 * Derive this: AvB
 ********************/
Expr* OrIntroduction(Expr* s1, Expr* s2) {
    return parse("(or " + s1 -> toString() + " " + s2 -> toString() + ")");
}

/*********************
 * From this: ¬¬A
 * Derive this: A
 ********************/
Expr* DoubleNegationElimination(Expr* s1) {
    vector<string> parts = tokenize(s1 -> toString());

    // dont consider the parts "(", "not", "(", "not", and the last 2 ")"
    vector<string> partsNeeded(parts.begin() + 4, parts.end() - 2);

    string sentence = "";
    for (int i = 0; i < partsNeeded.size(); i++) {
        sentence += partsNeeded[i] + " ";
    }
    
    return parse(sentence);
}

/*********************
 * From this: AvB, ¬AvC
 * Derive this: BvC
 ********************/
Expr* Resolution(Expr* s1, Expr* s2) {
    Expr* ret = nullptr;

    vector<string> s1Parts = tokenize(s1 -> toString());
    vector<string> s2Parts = tokenize(s2 -> toString());

    // like many other functions in this file, prepareing to extract A by removing irrelevant parts
    // dont need the "(", "or", and last ")" (in both token vectors)
    vector<string> s1PartsNeeded(s1Parts.begin() + 2, s1Parts.end() - 1);
    vector<string> s2PartsNeeded(s2Parts.begin() + 2, s2Parts.end() - 1);

    // first, we want to check if ¬A is actually A from s1, but negated
    string s1A = extractExpression(0, s1PartsNeeded) -> toString();
    string s1NegA = "(not " + s1A + ")";
    Expr* s2NegA = extractExpression(0, s2PartsNeeded);
    if (Eq(parse(s1NegA), s2NegA)) {

        // now we can extract B from s1 and C from s2 and or them together
        string s1B = extractExpression(1, s1PartsNeeded) -> toString();
        string s2C = extractExpression(1, s2PartsNeeded) -> toString();
        ret = parse("(or " + s1B + " " + s2C + ")");
    }

    return ret;
}

/*********************
 * 2 versions:
 * 1. 'not' over 'or':
 * From this: ¬(AvB)
 * Derive this: ¬A^¬B
 * 2. 'not' over 'and':
 * From this: ¬(A^B)
 * Derive this: ¬Av¬B
 ********************/
Expr* DeMorgan(Expr* s1) {
    // first, we want to get the version by seeing if we have "or" or "and"
    int version;
    vector<string> parts = tokenize(s1 -> toString());
    if (parts[3] == "or") version = 1;
    else version = 2;

    string A, B, negA, negB;

    // dont need "(", "not", "(", "and/or", and the last 2 ")"
    vector<string> partsNeeded(parts.begin() + 4, parts.end() - 2);

    // no matter the version, we must extract A and B and negate both of them separately
    A = extractExpression(0, partsNeeded) -> toString();
    negA = "(not " + A + ")";
    B = extractExpression(1, partsNeeded) -> toString();
    negB = "(not " + B + ")";

    // not over or
    if (version == 1) {
        return parse("(and " + negA + " " + negB + ")");
    }

    // not over and
    else {
        return parse("(or " + negA + " " + negB + ")");
    }
}


// Test functions
void testModusPonens(Expr* s1, Expr* s2) {
    Expr* s3 = ModusPonens(s1, s2);
    cout << s3 -> toString() << endl;
}

void testImplicationElimination(Expr* s1) {
    Expr* s2 = ImplicationElimination(s1);
    cout << s2 -> toString() << endl;
}

void testAndElimination(Expr* s1, int clause) {
    Expr* s2 = AndElimination(s1, clause);
    cout << s2 -> toString() << endl;
}

void testAndIntroduction(Expr* s1, Expr* s2) {
    Expr* s3 = AndIntroduction(s1, s2);
    cout << s3 -> toString() << endl;
}

void testOrIntroduction(Expr* s1, Expr* s2) {
    Expr* s3 = OrIntroduction(s1, s2);
    cout << s3 -> toString() << endl;
}

void testDoubleNegationElimination(Expr* s1) {
    Expr* s2 = DoubleNegationElimination(s1);
    cout << s2 -> toString() << endl;
}

void testResolution(Expr* s1, Expr* s2) {
    Expr* s3 = Resolution(s1, s2);
    cout << s3 -> toString() << endl;
}

void testDeMorgan(Expr* s1) {
    Expr* s2 = DeMorgan(s1);
    cout << s2 -> toString() << endl;
}
