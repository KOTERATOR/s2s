//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_FUNCTIONNODE_H
#define S2S_FUNCTIONNODE_H

#include "ModifiersNode.h"

class FunctionNode : public ParserNode {
public:
    std::string name;
    ModifiersNode *modifiers = nullptr;
    std::vector<ParserNode *> args;
    std::vector<ParserNode *> body;

    FunctionNode(std::string name, std::vector<ParserNode *> args, std::vector<ParserNode *> body) : ParserNode(
            NodeType::Function) {
        this->name = std::move(name);
        this->args = std::move(args);
        this->body = std::move(body);
    }

    std::string toString() override {
        std::string argsStr = "[";
        for (auto &a: args) {
            argsStr += a->toString() + ", ";
        }
        argsStr += "]";

        std::string bodyStr = "[";
        for (auto &a: body) {
            bodyStr += a->toString() + ", ";
        }
        bodyStr += "]";

        return std::string("Function ") + name + std::string(" args=") + argsStr + std::string(" body=") + bodyStr;
    }
};

#endif //S2S_FUNCTIONNODE_H
