#include <iostream>
#include <fstream>

#include "wordLogic/word_logic.h"

int main() {
    std::ifstream in("input.txt");
    std::string str;
    std::getline(in, str);

    std::cout << str << '\n';
    WordLogic *wordLogic = new WordLogic(str);
    std::cout << wordLogic->getExpressionStateMessage() << std::endl;
    wordLogic->printTree();
    
    delete wordLogic;
    return 0;
}