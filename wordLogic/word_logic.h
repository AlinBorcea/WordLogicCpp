#ifndef WORD_LOGIC_H
#define WORD_LOGIC_H

#include <iostream>
#include "expression.h"

class WordLogic : public Expression {
    private:
        // Binary tree.
        WordLogic *leftNode;
        WordLogic *rightNode;

        // Initializes the tree.
        void initTree() {
            std::string left, right;
            int doom;

            if ((doom = Expression::findDoom()) != -1) {
                left = Expression::getLeftOfDoom(doom);
                right = Expression::getRightOfDoom(doom);

                leftNode = new WordLogic(left);
                rightNode = new WordLogic(right);
                Expression::expr = Expression::expr[doom];

            } else if ((doom = Expression::findNext(0, CONNECTOR1_TYPE)) != -1) {
                left = Expression::expr.substr(doom + 1);
                Expression::expr = Expression::expr[doom];
                leftNode = new WordLogic(left);

            } else if ((doom = Expression::findNext(0, ATOM_TYPE)) != -1) {
                Expression::expr = Expression::expr[doom];
            }
        }

        void printNodes(WordLogic *tree) {
            if (!tree) return;
            std::cout << tree->expr << '\n';
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
        WordLogic(std::string expr) : Expression(expr) {
            leftNode = rightNode = 0;
            initTree();
        }

        ~WordLogic() {
            freeTree(this);
        }

        void printTree() {
            printNodes(this);
        }

        std::string getExpressionStateMessage() {
            switch (Expression::exprState) {
                case GOOD_EXPRESSION_STATE:
                    return "The expression is good";
                case BAD_CHAR_STATE:
                    return "Invalid char found";
                case PARENTHESES_MISSCOUNT_STATE:
                    return "Missing a parenthesis";
                case CONNECTORS_PARENTHESES_FAULT_STATE:
                    return "Connector parenthesis relation unfulfiled";
                case BAD_COMBO_STATE:
                    return "Expression does not follow every rule";
            }
            return "Something bad!";
        }

};

#endif