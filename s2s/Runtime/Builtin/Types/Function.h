//
// Created by iashi on 27.11.2021.
//

#ifndef S2S_FUNCTION_H
#define S2S_FUNCTION_H

#include <map>
#include "Type.h"
#include "Parser/Parser.h"
#include "Parser/Nodes/ParserNode.h"

class Block;
class Runtime;


class Function : public Type {
public:
    enum FunctionType {
        Global,
        Static,
        Member
    };

    std::string name;
    FunctionType functionType;
    std::vector<ParserNode *> args;
    std::vector<ParserNode *> body;
    Block *parentBlock;

    Type *cls = nullptr;
    NativeFunction nativeFunction = nullptr;

    Function(const std::string &name, FunctionType functionType = FunctionType::Global) : Type(Type::Function)
    {
        this->functionType = functionType;
        parentBlock = nullptr;
        this->name= name;
    }

    Function(const std::string &name, Type* cls, NativeFunction impl, FunctionType functionType = FunctionType::Global) : Type(Type::Function)
    {
        this->functionType = functionType;
        this->cls = cls;
        this->nativeFunction = impl;
        this->name = name;
    }

    Function(Block *parentBlock, FunctionNode *node, FunctionType functionType = FunctionType::Global) : Type(Type::Function)
    {
        this->functionType = functionType;
        this->parentBlock = parentBlock;
        name = node->name;
        args = node->args;
        body = node->body;
    }

    virtual Type *invoke(Runtime *r, Args args, KWArgs kwargs, Type *handle);

    std::string toString() override
    {
        return std::string("function ") + name;
    }
};


#endif //S2S_FUNCTION_H
