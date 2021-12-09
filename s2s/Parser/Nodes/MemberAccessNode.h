//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_MEMBERACCESSNODE_H
#define S2S_MEMBERACCESSNODE_H

#include "ParserNode.h"

class MemberAccessNode : public ParserNode {
public:
    ParserNode *object;
    ParserNode *member;

    MemberAccessNode(ParserNode *object, ParserNode *member) : ParserNode(NodeType::MemberAccess)
    {
        this->object = object;
        this->member = member;
    }

    std::string toString() override
    {
        return "MemberAccessNode: object: " + object->toString() + ", member: " + member->toString();
    }
};


#endif //S2S_MEMBERACCESSNODE_H
