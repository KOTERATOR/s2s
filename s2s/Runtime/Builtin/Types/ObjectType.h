//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_OBJECTTYPE_H
#define S2S_OBJECTTYPE_H

#include "ClassType.h"

class ObjectType : public Type {
private:
    ClassType *type;
public:
    ObjectType(ClassType *type) : Type(TypeEnum::Custom, (Block*)type)
    {
        this->type = type;
    }

    Type *get(Block *from, const std::string &name, bool allowParentScope) override {
        if (members.find(name) != members.end())
            return Block::get(from, name, false);
        else if (type->members.find(name) != type->members.end())
            return type->members[name];
        else
            throw RuntimeException("member \"" + name + "\" of object of type \"" + type->name + "\" not found");
    }

    void addMember(Block *from, const std::string &name, Type *member) override {
        Block::addMember(from, name, member);
    }

    void addMember(Block *from, const std::string &name, Modifiers mods, Type *member) override {
        Block::addMember(from, name, mods, member);
    }
};


#endif //S2S_OBJECTTYPE_H
