//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_VALUENODE_H
#define S2S_VALUENODE_H

#include "../../Lexer/Token.h"
#include "ParserNode.h"

class ValueNode : public ParserNode {
public:
    Token value;

    ValueNode(Token value) : ParserNode(Value) {
        this->value = value;
    }

    std::string toString() override {
        return std::string("ValueNode: value=") + value.toString();
    }
};


#endif //S2S_VALUENODE_H
