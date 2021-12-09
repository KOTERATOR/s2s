//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_SYMBOLNODE_H
#define S2S_SYMBOLNODE_H

#include "ParserNode.h"

class SymbolNode : public ParserNode {
public:
    std::string name;
    ModifiersNode *modifiers = nullptr;

    SymbolNode(std::string name) : ParserNode(Symbol) {
        this->name = name;
    }

    std::string toString() override {
        return std::string("SymbolNode: name=") + name;
    }
};

#endif //S2S_SYMBOLNODE_H
