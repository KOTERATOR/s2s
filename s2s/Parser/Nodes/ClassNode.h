//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_CLASSNODE_H
#define S2S_CLASSNODE_H

#include "ParserNode.h"
#include "ModifiersNode.h"

class ClassNode : public ParserNode
{
public:
    std::string name;
    ModifiersNode *modifiers = nullptr;
    std::vector<ParserNode*> body{};

    ClassNode(const std::string &name, const std::vector<ParserNode*> &body) : ParserNode(NodeType::Class)
    {
        this->name = name;
        this->body = body;
    }

    std::string toString() override
    {
        std::string bodyStr;
        for (int i = 0; i < body.size(); i++)
        {
            bodyStr += body[i]->toString();
            if (i < body.size()-1)
                bodyStr += ", ";
        }
        return "ClassNode: name=" + name + ", body=" + bodyStr;
    }
};

#endif //S2S_CLASSNODE_H
