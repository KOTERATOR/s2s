//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_OPERATIONNODE_H
#define S2S_OPERATIONNODE_H

#include "ParserNode.h"

class OperationNode : public ParserNode {
public:
    std::string op;
    ParserNode *op1, *op2;

    OperationNode(std::string op, ParserNode *op1, ParserNode *op2) : ParserNode(NodeType::Operation) {
        this->op = std::move(op);
        this->op1 = op1;
        this->op2 = op2;
    }

    OperationNode *insert(const std::string &nop, ParserNode *nop1) {
        if (op == "*" || op == "/") {
            if (nop == "+" || nop == "-") {
                return new OperationNode(nop, nop1, this);
            } else {
                if (this->op1->type == NodeType::Operation) {
                    this->op1 = ((OperationNode *) this->op1)->insert(nop, nop1);
                } else {
                    auto *n = new OperationNode(nop, nop1, this->op1);
                    this->op1 = n;
                }

                return this;
            }
        } else {
            if (nop == "*" || nop == "/") {
                if (this->op1->type == NodeType::Operation) {
                    this->op1 = ((OperationNode *) this->op1)->insert(nop, nop1);
                } else {
                    auto *n = new OperationNode(nop, nop1, this->op1);
                    this->op1 = n;
                }
                return this;
            } else {
                if (this->op1->type == NodeType::Operation) {
                    this->op1 = ((OperationNode *) this->op1)->insert(nop, nop1);
                } else {
                    auto *n = new OperationNode(nop, nop1, this->op1);
                    this->op1 = n;
                }
                return this;
            }
        }
    }

    std::string toString() override {
        return std::string("OperationNode: op = ") + op + std::string("{ op1: ") + op1->toString() +
               std::string("} { op2: ") + op2->toString() + std::string(" }");
    }
};


#endif //S2S_OPERATIONNODE_H
