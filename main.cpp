#include <iostream>
#include <fstream>

#include "wordLogic/word_logic.h"

void initInterpretation(std::string interp, bool interpretation[], int len) {
    for (int i = 0; i < len; i++)
        interpretation[i] = interp[i] == '1';
}

int main() {
    std::ifstream in("input.txt");
    std::string expr;
    std::string interp;
    std::getline(in, expr);
    std::getline(in, interp);
    bool interpretation[26];
    int len = 26;

    initInterpretation(interp, interpretation, len);

    std::cout << expr << '\n';
    WordLogic *wordLogic = new WordLogic(expr);
    wordLogic->setInterpretation(interpretation);

    std::cout << wordLogic->getExpressionStateMessage() << std::endl;
    std::cout << "Truth value: " << wordLogic->getTruthValue() << '\n';
    wordLogic->printTree();
    
    delete wordLogic;
    return 0;
}