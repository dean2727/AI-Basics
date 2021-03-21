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
#include <fstream>
using namespace std;

void do_inference(vector<Expr*>& KB) {
    ofstream os("sammys.txt");

    // 1. and intro (args: L3B, O3Y)
    KB.push_back(AndIntroduction(KB[56], KB[55]));
    os << KB[57]->toString() << endl;

    // 2. modus ponens (args: L3B ^ O3Y -> C3Y, L3B ^ O3Y)
    KB.push_back(ModusPonens(KB[29], KB[57]));
    os << KB[58]->toString() << endl;

    // 3. and intro (args: L1W, O1Y)
    KB.push_back(AndIntroduction(KB[52], KB[51]));
    os << KB[59]->toString() << endl;

    // 4. modus ponens (args: L1W ^ O1Y -> C1Y v C1B, L1W ^ O1Y)
    KB.push_back(ModusPonens(KB[24], KB[59]));
    os << KB[60]->toString() << endl;

    // 5. implication elimination (args: C1Y -> ¬C3Y)
    KB.push_back(ImplicationElimination(KB[1]));
    os << KB[61]->toString() << endl;

    // 6. resolution (args: C1Y v C1B, ¬C1Y v ¬C3Y)
    KB.push_back(Resolution(KB[60], KB[61]));
    os << KB[62]->toString() << endl;

    // 7. implication introduction (args: C1B v ¬C3Y)
    KB.push_back(ImplicationIntroduction(KB[62]));
    os << KB[63]->toString() << endl;

    // 8. modus tolens (args: ¬C1B -> ¬C3Y, C3Y)
    KB.push_back(ModusTolens(KB[63], KB[58]));
    os << KB[64]->toString() << endl;

    // 9. and intro (args: C1B, C3Y)
    KB.push_back(AndIntroduction(KB[64], KB[58]));
    os << KB[65]->toString() << endl;

    // 10. modus ponens (args: C1B ^ C3Y -> C2W, C1B ^ C3Y)
    KB.push_back(ModusPonens(KB[42], KB[65]));
    os << KB[66]->toString() << endl;

    os.close();
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
