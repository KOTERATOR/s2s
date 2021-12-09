//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_ASSIGNMENTNODE_H
#define S2S_ASSIGNMENTNODE_H

#include "ParserNode.h"

class AssignmentNode : public ParserNode {
public:
    ParserNode *to;
    ParserNode *what;

    AssignmentNode(ParserNode *to, ParserNode *what) : ParserNode(NodeType::Assignment) {
        this->to = to;
        this->what = what;
    }

    std::string toString() override {
        return std::string("AssignmentNode:\n\tto: ") + to->toString() + std::string("\n\twhat: ") + what->toString();
    }
};


#endif //S2S_ASSIGNMENTNODE_H
