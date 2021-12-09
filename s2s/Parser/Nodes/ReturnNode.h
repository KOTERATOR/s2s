//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_RETURNNODE_H
#define S2S_RETURNNODE_H

#include "ParserNode.h"

class ReturnNode : public ParserNode {
public:
    ParserNode *what;

    ReturnNode(ParserNode *what) : ParserNode(NodeType::Return) {
        this->what = what;
    }

    std::string toString() override {
        return std::string("Return Node: ") + what->toString();
    }
};



#endif //S2S_RETURNNODE_H
