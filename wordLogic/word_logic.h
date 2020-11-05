#ifndef WORD_LOGIC_H
#define WORD_LOGIC_H

#include <iostream>
#include "expression.h"

class WordLogic : public Expression {
    private:
        // Binary tree.
        WordLogic *leftNode;
        WordLogic *rightNode;
        bool interp[26];

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

        bool evaluate(WordLogic *tree, bool interpretation[26]) {
            if (elementType(tree->expr[0]) == ATOM_TYPE)
                return interpretation[(int) tree->expr[0] - 65];

            switch (tree->expr[0]) {
                case NEGATION:
                    return !evaluate(tree->leftNode, interpretation);
                case CONJUNCTION:
                    return evaluate(tree->leftNode, interpretation) && evaluate(tree->rightNode, interpretation);
                case DISJUNCTION:
                    return evaluate(tree->leftNode, interpretation) || evaluate(tree->rightNode, interpretation);
                case IMPLICATION:
                    return !evaluate(tree->leftNode, interpretation) || evaluate(tree->rightNode, interpretation);
                case EQUIVALENCE:
                    bool F = evaluate(tree->leftNode, interpretation);
                    bool G = evaluate(tree->rightNode, interpretation);
                    return (!F || G) && (!G || F);

            }
            return true; // It should not get here!
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

        void setInterpretation(bool interpretation[26]) {
            for (int i = 0; i < 26; i++)
                interp[i] = interpretation[i];
        }

        void printTree() {
            printNodes(this);
        }

        bool getTruthValue() {
            return evaluate(this, interp);
        }

};

#endif