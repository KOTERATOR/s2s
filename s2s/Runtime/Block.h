//
// Created by iashi on 27.11.2021.
//

#ifndef S2S_BLOCK_H
#define S2S_BLOCK_H

#include <map>
#include <string>
#include "../Types/Type.h"

class Function;

class Block
{
public:
    Block *parent = nullptr;
    std::map<std::string, Type *> variables;
    std::map<std::string, Function *> functions;

    Block(Block *parent = nullptr) : parent(parent) {}

    Type *getVariable(const std::string& name)
    {
        if (variables.find(name) != variables.end())
            return variables[name];
        else if (parent != nullptr)
            return parent->getVariable(name);
        else
            return nullptr; // maybe throw ???
    }

    void addVariable(const std::string &name, Type* value)
    {
        variables[name] = value;
    }

    Function *getFunction(const std::string& name)
    {
        if (functions.find(name) != functions.end())
            return functions[name];
        else if (parent != nullptr)
            return parent->getFunction(name);
        else
            return nullptr;
    }

    void addFunction(const std::string &name, Function *f)
    {
        functions[name] = f;
    }

    void addFunction(Function *f);
};


#endif //S2S_BLOCK_H
