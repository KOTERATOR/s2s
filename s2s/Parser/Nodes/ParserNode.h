//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_PARSERNODE_H
#define S2S_PARSERNODE_H


#include <string>


class ParserNode {
public:
    enum NodeType {
        Function,
        FunctionCall,
        Task,
        TaskCall,
        Class,
        If,
        GetOperation,
        Operation,
        CompareOperation,
        BitOperation,
        Parentheses,
        Symbol,
        Value,
        Array,
        Assignment,
        Return,
        MemberAccess,
        Modifiers
    };

    NodeType type;

    ParserNode(NodeType type) : type(type) {
    }

    virtual ~ParserNode() {
    }

    virtual std::string toString() {
        return std::string("ParserNode::toString");
    }
};

#endif //S2S_PARSERNODE_H
