//
// Created by iashi on 27.11.2021.
//

#ifndef S2S_FUNCTION_H
#define S2S_FUNCTION_H

#include "Type.h"
#include "../Parser/Parser.h"

class Block;
class Runtime;

class Function : public Type {
public:
    std::string name;
    std::vector<ParserNode *> args;
    std::vector<ParserNode *> body;
    Block *parentBlock;

    Function(const std::string &name) : Type(Type::Function)
    {
        parentBlock = nullptr;
        this->name= name;
    }

    Function(Block *parentBlock, FunctionNode *node) : Type(Type::Function)
    {
        this->parentBlock = parentBlock;
        name = node->name;
        args = node->args;
        body = node->body;
    }

    virtual Type *invoke(Runtime *r, std::vector<Type *> args);

    std::string toString() override
    {
        return std::string("function ") + name;
    }
};


#endif //S2S_FUNCTION_H
