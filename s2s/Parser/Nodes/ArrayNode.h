//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_ARRAYNODE_H
#define S2S_ARRAYNODE_H

#include "ParserNode.h"

class ArrayNode : public ParserNode {
public:
    std::vector<ParserNode *> elements;

    ArrayNode(std::vector<ParserNode *> elements) : ParserNode(NodeType::Array) {
        this->elements = std::move(elements);
    }

    std::string toString() override {
        std::string result = std::string("ArrayNode: elements=[");
        for (int i = 0; i < elements.size(); i++)
            result += elements[i]->toString();
        result += "]";
        return result;
    }
};

#endif //S2S_ARRAYNODE_H
