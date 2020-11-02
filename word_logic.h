#ifndef WORD_LOGIC_H
#define WORD_LOGIC_H

#include <iostream>

// Types of symbols in expression
#define NO_TYPE 0
#define ATOM_TYPE 1
#define LEFT_PARENTHESES_TYPE 2
#define RIGHT_PARENTHESES_TYPE 3
#define CONNECTOR1_TYPE 4
#define CONNECTOR2_TYPE 5

// Connectors with one operand
#define NEGATION 'n'

// Connectors with two operands
#define CONJUNCTION 'c'
#define DISJUNCTION 'd'
#define IMPLICATION 'i'
#define EQUIVALENCE 'e'

class WordLogic {
    private:

        // Binary tree.
        std::string expression;
        WordLogic *leftNode;
        WordLogic *rightNode;

        // Expression properties.
        int rightParentheses;
        int leftParentheses;
        int atoms;
        int negations;
        int connectors;

        // Initializes the tree.
        void initTree() {
            std::string left, right;
            int doom = findDoom();

            if (doom != -1) {
                left = expression.substr(1, doom - 1);
                right = expression.substr(doom + 1, expression.size() - 1 - doom);

                leftNode = new WordLogic(left);
                rightNode = new WordLogic(right);
            }
        }

        void initExpressionProperties() {
            for (std::string::iterator it = expression.begin(); it != expression.end(); ++it) {
                int elemType = elementType(*it);
                if (elemType == ATOM_TYPE)
                    atoms++;
                else if (elemType == LEFT_PARENTHESES_TYPE)
                    leftParentheses++;
                else if (elemType == RIGHT_PARENTHESES_TYPE)
                    rightParentheses++;
                else if (elemType == CONNECTOR1_TYPE)
                    negations++;
                else if (elemType == CONNECTOR2_TYPE)
                    connectors++;
            }
        }

        // Finds the dominant connector.
        int findDoom() {
            int parentheses = 0;
            int elemType;

            for (int i = 0; i < expression.size(); i++) {
                elemType = elementType(expression[i]);

                if (elemType == CONNECTOR2_TYPE && parentheses == 1) 
                    return i;
                
                else if (elemType == LEFT_PARENTHESES_TYPE)
                    parentheses++;

                else if (elemType == RIGHT_PARENTHESES_TYPE)
                    parentheses--;
            }
            return -1;
        }

        int elementType(char element) {
            if (element >= 65 && element <= 96)
                return ATOM_TYPE;

            if (element == '(')
                return LEFT_PARENTHESES_TYPE;

            if (element == ')')
                return RIGHT_PARENTHESES_TYPE;

            if (element == NEGATION)
                return CONNECTOR1_TYPE;

            if (element == 'c' || element == 'd' || element == 'i' || element == 'e')
                return CONNECTOR2_TYPE;

            return NO_TYPE;
        }

        // Returns true if two consecutive elements follow order rules.
        bool isCombo(int currentType, int nextType) {
            
            switch (currentType) {

                case LEFT_PARENTHESES_TYPE:
                    if (nextType == RIGHT_PARENTHESES_TYPE || nextType == CONNECTOR2_TYPE)
                        return false;
                    break;

                case RIGHT_PARENTHESES_TYPE:
                    if (nextType == ATOM_TYPE || nextType == NEGATION || nextType == LEFT_PARENTHESES_TYPE)
                        return false;
                    break;

                case ATOM_TYPE:
                    if (nextType == LEFT_PARENTHESES_TYPE || nextType == ATOM_TYPE || nextType == NEGATION)
                        return false;
                    break;

                case CONNECTOR1_TYPE:
                    if (nextType == RIGHT_PARENTHESES_TYPE || nextType == CONNECTOR2_TYPE)
                        return false;
                    break;

                case CONNECTOR2_TYPE:
                    if (nextType == CONNECTOR2_TYPE)
                        return false;
                    break;

                case NO_TYPE:
                    return false;

                default:
                    return true;

            }
            return true;
        }

        void printNodes(WordLogic *tree) {
            if (!tree) return;
            std::cout << tree->expression << '\n';
            printNodes(tree->leftNode);
            printNodes(tree->rightNode);
        }

        void freeTree(WordLogic *tree) {
            if (!tree) return;
            freeTree(tree->leftNode);
            freeTree(tree->rightNode);
            delete tree;
        }

    public:

        // Constructor.
        WordLogic(std::string expr) {
            
            expression = expr;
            leftNode = rightNode = 0;

            rightParentheses = 0;
            leftParentheses = 0;
            atoms = 0;
            negations = 0;
            connectors = 0;
            
            initExpressionProperties();
            initTree();
        }

        ~WordLogic() {
            freeTree(this);
        }

        /* Checks if the expression follows the general rules.
         * return value: true if the expression folows the rules or false otherwise.
         * leftParentheses must be equal to rightParentheses.
         * The expression must not be empty and it must start with '(' and end with ')'.
         * Neighboring characters must be a combo (see isCombo()).
        */
        bool isExpression() {

            if (leftParentheses == 0 && rightParentheses == 0 && atoms == 1)
                return true;
            
            if (rightParentheses != leftParentheses)
                return false;
            
            if (expression.empty() || expression.front() != '(' || expression.back() != ')')
                return false;

            if (connectors + negations != leftParentheses)
                return false;
            
            std::string::iterator it1, it2;
            int currentType;
            int nextType;

            it1 = expression.begin();
            it2 = it1 + 1;

            while (it1 != expression.end() - 1) {
                currentType = elementType(*it1);
                nextType = elementType(*it2);

                if (currentType == NO_TYPE || nextType == NO_TYPE)
                    return false;

                if (!isCombo(currentType, nextType))
                    return false;

                it1 = it2;
                ++it2;
            }

            return true;
        }

        void printTree() {
            printNodes(this);
        }
};

#endif