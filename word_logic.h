#ifndef WORD_LOGIC_H
#define WORD_LOGIC_H

#include <iostream>

// Connectors with one operand
#define NEGATION 'n'

// Connectors with two operands
#define CONJUNCTION 'c'
#define DISJUNCTION 'd'
#define IMPLICATION 'i'
#define EQUIVALENCE 'e'

// Types
#define NO_TYPE 0
#define ATOM_TYPE 1
#define LEFT_PARENTHESES_TYPE 2
#define RIGHT_PARENTHESES_TYPE 3
#define CONNECTOR1_TYPE 4
#define CONNECTOR2_TYPE 5

class WordLogic {
    private:
        std::string expression;
        int rightParentheses;
        int leftParentheses;
        int atoms;
        int negations;
        int connectors;

        void initData() {
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

        bool isCombo(int currentType, int nextType) {
            
            switch (currentType) {

                case LEFT_PARENTHESES_TYPE: // Case 1 '('
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

                default:
                    return true;

            }
            return true;
        }

    public:
        WordLogic(std::string expr) {
            expression = expr;
            rightParentheses = 0;
            leftParentheses = 0;
            atoms = 0;
            negations = 0;
            connectors = 0;
            initData();
        }

        std::string getExpression() {
            return expression;
        }

        bool isExpression() {
            // if we have no string or first element is not '(' or last element is not ')' then expression is not an expression
            if (leftParentheses == rightParentheses == 0 && atoms == 1)
                return true;
            
            if (expression.empty() || expression.front() != '(' || expression.back() != ')')
                return false;

            if (rightParentheses != leftParentheses)
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

};

#endif