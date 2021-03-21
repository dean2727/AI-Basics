// Testing suite for testing the various rules of inference

#include "parser.hpp"
#include "NatDed.cpp"
using namespace std;

int main () {
    try {
        vector<Expr*> KB = load_kb("test.kb");
        //show_kb(KB);

        cout << "Testing and intro:\n";
        cout << KB[0] -> toString() << ", " << KB[1] -> toString() << " ---> ";
        testAndIntroduction(KB[0], KB[1]);
        cout << endl;
        cout << KB[0] -> toString() << ", " << KB[2] -> toString() << " ---> ";
        testAndIntroduction(KB[0], KB[2]);
        cout << endl;

        cout << "Testing or intro:\n";
        cout << KB[0] -> toString() << ", " << KB[1] -> toString() << " ---> ";
        testOrIntroduction(KB[0], KB[1]);
        cout << endl;
        cout << KB[0] -> toString() << ", " << KB[2] -> toString() << " ---> ";
        testOrIntroduction(KB[0], KB[2]);
        cout << endl;
        
        cout << "Testing double neg elimination:\n";
        cout << KB[3] -> toString() << " ---> ";
        testDoubleNegationElimination(KB[3]);
        cout << endl;
        cout << KB[4] -> toString() << " ---> ";
        testDoubleNegationElimination(KB[4]);
        cout << endl;

        cout << "Testing and elimination:\n";
        cout << KB[5] -> toString() << "(clause 1) ---> ";
        testAndElimination(KB[5], 1);
        cout << endl;
        cout << KB[5] -> toString() << "(clause 2) ---> ";
        testAndElimination(KB[5], 2);
        cout << endl;
        cout << KB[6] -> toString() << "(clause 1) ---> ";
        testAndElimination(KB[6], 1);
        cout << endl;
        cout << KB[6] -> toString() << "(clause 2) ---> ";
        testAndElimination(KB[6], 2);
        cout << endl;
        cout << KB[7] -> toString() << "(clause 1) ---> ";
        testAndElimination(KB[7], 1);
        cout << endl;
        cout << KB[7] -> toString() << "(clause 2) ---> ";
        testAndElimination(KB[7], 2);
        cout << endl;
        cout << KB[8] -> toString() << "(clause 1) ---> ";
        testAndElimination(KB[8], 1);
        cout << endl;
        cout << KB[8] -> toString() << "(clause 2) ---> ";
        testAndElimination(KB[8], 2);
        cout << endl;

        cout << "Testing modus ponens:\n";
        cout << KB[13] -> toString() << ", " << KB[0] -> toString() << " ---> ";
        testModusPonens(KB[13], KB[0]);
        cout << endl;
        cout << KB[16] -> toString() << ", " << KB[14] -> toString() << " ---> ";
        testModusPonens(KB[16], KB[14]);
        cout << endl;
        cout << KB[17] -> toString() << ", " << KB[18] -> toString() << " ---> ";
        testModusPonens(KB[17], KB[18]);
        cout << endl;

        cout << "Testing implication elimination:\n";
        cout << KB[13] -> toString() << " ---> ";
        testImplicationElimination(KB[13]);
        cout << endl;
        cout << KB[17] -> toString() << " ---> ";
        testImplicationElimination(KB[17]);
        cout << endl;

        cout << "Testing resolution:\n";
        cout << KB[9] -> toString() << ", " << KB[19] -> toString() << " ---> ";
        testResolution(KB[9], KB[19]);
        cout << endl;
        cout << KB[20] -> toString() << ", " << KB[21] -> toString() << " ---> ";
        testResolution(KB[20], KB[21]);
        cout << endl;

        cout << "Testing DeMorgan:\n";
        cout << "Version 1:\n";
        cout << KB[22] -> toString() << " ---> ";
        testDeMorgan(KB[22]);
        cout << endl;
        cout << KB[23] -> toString() << " ---> ";
        testDeMorgan(KB[23]);
        cout << endl;
        cout << "Version 2:\n";
        cout << KB[24] -> toString() << " ---> ";
        testDeMorgan(KB[24]);
        cout << endl;
        cout << KB[25] -> toString() << " ---> ";
        testDeMorgan(KB[25]);
        cout << endl;

        cout << "Testing implication introduction:\n";
        cout << KB[19]->toString() << " ---> ";
        testImplicationIntroduction(KB[19]);
        cout << endl;
        cout << KB[20]->toString() << " ---> ";
        testImplicationIntroduction(KB[20]);
        cout << endl;

        cout << "Testing modus tolens:\n";
        cout << KB[26] -> toString() << ", " << KB[27] -> toString() << " ---> ";
        testModusTolens(KB[26], KB[27]);
        cout << endl;
        cout << KB[28] -> toString() << ", " << KB[29] -> toString() << " ---> ";
        testModusTolens(KB[28], KB[29]);
        cout << endl;
    }
    
    catch (runtime_error& e) {
        cout << e.what() << endl;
    }
}