//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_OBJECTTYPE_H
#define S2S_OBJECTTYPE_H

#include "ClassType.h"

class ObjectType : public Type {
public:
    ObjectType(ClassType *type) : Type(TypeEnum::Custom, (Block*)type)
    {
        this->type = type;
    }

    void Construct(Runtime *r, Args args, KWArgs kwargs) {
        if (hasMember("Constructor", true)) {
            callMethod(r, "Constructor", args, kwargs);
        }
    }

    Type *&get(Block *from, const std::string &name, bool allowParentScope) override {
        if (members.find(name) != members.end())
            return Block::get(from, name, false);
        else if (type->members.find(name) != type->members.end())
            return type->getMember(from, name);
        else if (parent != nullptr)
            return parent->get(from, name);
        else
            throw RuntimeException("member \"" + name + "\" of object of type \"" + type->name + "\" not found");
    }

    void addMember(Block *from, const std::string &name, Type *member) override {
        Block::addMember(from, name, member);
    }

    void addMember(Block *from, const std::string &name, Modifiers mods, Type *member) override {
        Block::addMember(from, name, mods, member);
    }

    Type *callMethod(Runtime *r, const std::string &name, Args args, KWArgs kwargs);

    ClassType *type;
};


#endif //S2S_OBJECTTYPE_H
