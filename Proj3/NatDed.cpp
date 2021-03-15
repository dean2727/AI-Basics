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
using namespace std;

/*********************
 * From this: A, A->B
 * Derive this: B
 ********************/
Expr* ModusPonens(Expr* s1, Expr* s2) {

}

/*********************
 * From this: A->B
 * Derive this: ¬AvB
 ********************/
Expr* ImplicationElimination(Expr* s1) {
    
}

/*********************
 * From this: A^B
 * Derive this: A
 ********************/
Expr* AndElimination(Expr* s1) {
    
}

/*********************
 * From this: A, B
 * Derive this: A^B
 ********************/
Expr* AndIntroduction(Expr* s1, Expr* s2) {
    
}

/*********************
 * From this: A, B
 * Derive this: AvB
 ********************/
Expr* OrIntroduction(Expr* s1, Expr* s2) {
    
}

/*********************
 * From this: ¬¬A
 * Derive this: A
 ********************/
Expr* DoubleNegationElimination(Expr* s1) {
    
}

/*********************
 * From this: AvB, ¬AvC
 * Derive this: BvC
 ********************/
Expr* Resolution(Expr* s1, Expr* s2) {
    
}

/*********************
 * From this: A, A->B
 * Derive this: B
 ********************/
Expr* DeMorgan(Expr* s1, Expr* s2) {
    
}


// Test functions
void testModusPonens(Expr* s1, Expr* s2) {
    Expr* s3 = ModusPonens(s1, s2); // args: P->Q, and P
    cout << s3 -> toString(); // output: Q
}

void testImplicationElimination(Expr* s1) {
    
}

void testAndElimination(Expr* s1) {
    
}

void testAndIntroduction(Expr* s1, Expr* s2) {
    
}

void testOrIntroduction(Expr* s1, Expr* s2) {
    
}

void testDoubleNegationElimination(Expr* s1) {
    
}

void testResolution(Expr* s1, Expr* s2) {
    
}

void testDeMorgan(Expr* s1) {
    
}
