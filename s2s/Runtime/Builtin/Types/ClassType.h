//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_CLASSTYPE_H
#define S2S_CLASSTYPE_H

#include "Type.h"
#include "Function.h"
#include "Runtime/Block.h"

class ObjectType;

class ClassType : public Type {
public:
    std::string name;
    std::vector<AssignmentNode*> instanceVars;

    ClassType(Block *parentBlock) : Type(Type::Class, parentBlock) {}
    ClassType(Runtime *r, Block *parentBlock, ClassNode *classNode);

    virtual ObjectType *createInstance(Runtime *r);

    Type *&get(Block *from, const std::string &name, bool allowParentScope) override;
    Type *&getMember(Block *from, const std::string &name);

    void addMember(Block *from, const std::string &name, Type *member) override;

    void addMember(Block *from, const std::string &name, Modifiers mods, Type *member) override;
};


#endif //S2S_CLASSTYPE_H
