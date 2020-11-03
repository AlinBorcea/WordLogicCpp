#include <iostream>
#include <fstream>

#include "word_logic.h"

int main() {
    std::ifstream in("input.txt");
    std::string str;
    std::getline(in, str);

    WordLogic *wordLogic = new WordLogic(str);
    std::cout << wordLogic->getExpressionStateMessage() << std::endl;
    wordLogic->printTree();
    
    delete wordLogic;
    return 0;
}