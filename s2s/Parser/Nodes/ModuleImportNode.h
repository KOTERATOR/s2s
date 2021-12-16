//
// Created by iashi on 15.12.2021.
//

#ifndef S2S_MODULEIMPORTNODE_H
#define S2S_MODULEIMPORTNODE_H

#include "ParserNode.h"

class ModuleImportNode : public ParserNode {
public:
    ParserNode *module;
    ModuleImportNode(ParserNode *module) : ParserNode(NodeType::ModuleImport), module(module)
    {

    }

    std::string toString() override {
        return "ModuleImportNode: " + module->toString();
    }
};

#endif //S2S_MODULEIMPORTNODE_H
