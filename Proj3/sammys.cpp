/***********************************************************************
 * File name: sammys.cpp
 * Sammys Sport Shop problem:
 * There is a shipment of 3 boxes filled with tennis balls. 1 box contains only
 * yellow tennis balls, 1 box contains only white tennis balls, and 1 box 
 * contains both yellow and white tennis balls. Each box is definitely 
 * labeled wrong. 1 ball is drawn from each box and observed. Given the
 * incorrect labeling and the 3 observations, use propositional logic to
 * derive the correct labeling of the middle box
 * Program description: Contains the function for performing the natural deduction
 * on the knowledge base for Sammys Sport Shop. Also contains the main
 * program, which will load the .kb file (knowledge base) and call the
 * inference function. Our goal is to prove that box 2 must contain white
 * balls (C2W)
 * Name: Dean Orenstein
 * Class: CSCE 420
 * Date: 03/22/2021
 **********************************************************************/

#include "parser.hpp"
#include "NatDed.cpp"
using namespace std;

void do_inference(vector<Expr*>& KB) {
    
}


int main() {
    try {
        vector<Expr*> KB = load_kb("sammys.kb");
        do_inference(KB);
    }
    catch (runtime_error& e) {
        cout << e.what() << endl;
    }
}
