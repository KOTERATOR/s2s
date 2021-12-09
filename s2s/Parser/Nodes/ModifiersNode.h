//
// Created by iashi on 08.12.2021.
//

#ifndef S2S_MODIFIERSNODE_H
#define S2S_MODIFIERSNODE_H

#include "ParserNode.h"
#include <utility>
#include <vector>
#include <string>

class ModifiersNode : public ParserNode
{
public:
    std::vector<std::string> modifiers;

    ModifiersNode(std::vector<std::string>  modifiers) : ParserNode(NodeType::Modifiers), modifiers(std::move(modifiers))
    {

    }

    std::string toString() override
    {
        std::string mods = "[";
        for (size_t i = 0; i < modifiers.size(); i++)
        {
            mods += modifiers[i];
            if (i < modifiers.size()-1)
                mods += ", ";
        }
        mods += "]";
        return "ModifiersNode: modifiers=" + mods;
    }
};

#endif //S2S_MODIFIERSNODE_H
