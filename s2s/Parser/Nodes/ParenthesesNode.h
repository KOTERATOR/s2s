//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_PARENTHESESNODE_H
#define S2S_PARENTHESESNODE_H

#include "ParserNode.h"

class ParenthesesNode : public ParserNode {
public:
    ParserNode *inner;

    ParenthesesNode(ParserNode *inner) : ParserNode(NodeType::Parentheses) {
        this->inner = inner;
    }

    std::string toString() override {
        return std::string("ParenthesesNode: inner = ") + inner->toString();
    }
};


#endif //S2S_PARENTHESESNODE_H
