#include <iostream>
#include "word_logic.h"

int main() {
    std::string str;
    std::cout << "Expression\n";
    std::cin >> str;
    
    WordLogic *wordLogic = new WordLogic(str);
    std::cout << wordLogic->isExpression() << std::endl;
    
    wordLogic->printTree();
    free(wordLogic);
    return 0;
}