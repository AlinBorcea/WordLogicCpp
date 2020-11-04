#ifndef EXPRESSION_H
#define EXPRESSION_H

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

// Expression states
#define GOOD_EXPRESSION_STATE 0
#define BAD_CHAR_STATE 1
#define PARENTHESES_MISSCOUNT_STATE 2
#define FIRST_LAST_FAULT_STATE 3
#define CONNECTORS_PARENTHESES_FAULT_STATE 4
#define BAD_COMBO_STATE 5

class Expression {
    protected:
        // Expression and properties.
        std::string expr;
        int rightParentheses;
        int leftParentheses;
        int atoms;
        int negations;
        int connectors;
        int exprState;

    private:
        void initExpressionProperties() {
            for (std::string::iterator it = expr.begin(); it != expr.end(); ++it) {
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

    public:
        Expression(std::string expression) {
            expr = expression;
            rightParentheses = 0;
            leftParentheses = 0;
            atoms = 0;
            negations = 0;
            connectors = 0;
            initExpressionProperties();
            exprState = expressionState();
        }

        // Finds the dominant connector.
        int findDoom() {
            int parentheses = 0;
            int elemType;

            for (int i = 0; i < expr.size(); i++) {
                elemType = elementType(expr[i]);

                if (elemType == CONNECTOR2_TYPE && parentheses == 1) 
                    return i;
                
                else if (elemType == LEFT_PARENTHESES_TYPE)
                    parentheses++;

                else if (elemType == RIGHT_PARENTHESES_TYPE)
                    parentheses--;
            }
            return -1;
        }

        int findNext(int start, int type) {
            while (start < expr.size() && elementType(expr[start]) != type)
                start++;
            return start > expr.size() ? -1 : start;
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

        /* Checks if the expression follows the general rules.
         * return value: true if the expression folows the rules or false otherwise.
         * leftParentheses must be equal to rightParentheses.
         * The expression must not be empty and it must start with '(' and end with ')'.
         * Neighboring characters must be a combo (see isCombo()).
        */
        int expressionState() {

            if (leftParentheses == 0 && rightParentheses == 0 && atoms == 1)
                return GOOD_EXPRESSION_STATE;
            
            if (rightParentheses != leftParentheses)
                return PARENTHESES_MISSCOUNT_STATE;
            
            if (expr.empty() || expr.front() != '(' || expr.back() != ')')
                return FIRST_LAST_FAULT_STATE;

            if (connectors + negations != leftParentheses)
                return CONNECTORS_PARENTHESES_FAULT_STATE;
            
            std::string::iterator it1, it2;
            int currentType;
            int nextType;

            it1 = expr.begin();
            it2 = it1 + 1;

            while (it1 != expr.end() - 1) {
                currentType = elementType(*it1);
                nextType = elementType(*it2);

                if (currentType == NO_TYPE || nextType == NO_TYPE)
                    return BAD_CHAR_STATE;

                if (!isCombo(currentType, nextType))
                    return BAD_COMBO_STATE;

                it1 = it2;
                ++it2;
            }
            return GOOD_EXPRESSION_STATE;
        }

};

#endif