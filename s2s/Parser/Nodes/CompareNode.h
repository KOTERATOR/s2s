//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_COMPARENODE_H
#define S2S_COMPARENODE_H

#include "ParserNode.h"

class CompareNode : public ParserNode {
public:
    enum CompareOperation {
        EQUALS,
        NOT_EQUALS,
        GT,
        GTE,
        LT,
        LTE
    };

    CompareOperation op;
    ParserNode *op1;
    ParserNode *op2;

    CompareNode(CompareOperation op, ParserNode *op1, ParserNode *op2) : ParserNode(NodeType::CompareOperation) {
        this->op = op;
        this->op1 = op1;
        this->op2 = op2;
    }

    std::string toString() override {
        return std::string("CompareNode: op = ") + std::to_string((int) op) + std::string(" op1: ") + op1->toString() +
               std::string(" op2: ") + op2->toString();
    }
};

#endif //S2S_COMPARENODE_H
