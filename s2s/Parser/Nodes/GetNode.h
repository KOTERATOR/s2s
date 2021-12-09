//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_GETNODE_H
#define S2S_GETNODE_H

#include "ParserNode.h"

class GetNode : public ParserNode {
public:
    ParserNode *what;
    ParserNode *value;

    GetNode(ParserNode *what, ParserNode *value) : ParserNode(NodeType::GetOperation) {
        this->what = what;
        this->value = value;
    }

    std::string toString() override {
        return std::string("Get Node: what: ") + what->toString() + std::string(", to: ") + value->toString();
    }
};


#endif //S2S_GETNODE_H
