//
// Created by iashi on 27.11.2021.
//

#ifndef S2S_BLOCK_H
#define S2S_BLOCK_H

#include <map>
#include <string>
#include <vector>
#include "RuntimeException.h"

class Type;
class Function;
class ClassType;

class Block
{
protected:
    std::vector<Block*> children;
public:
    enum AccessModifiers
    {
        Public,
        Private,
        Protected,
        Default
    };

    struct Modifiers
    {
        AccessModifiers accessModifier = AccessModifiers::Default;
        bool isConst = false;
        bool isStatic = false;
    };

    bool isTypeBlock = false;
    Block *parent = nullptr;
    std::map<std::string, Type *> members;
    std::map<std::string, Modifiers> modifiers;

    Block(Block *parent = nullptr) : parent(parent)
    {
        if (parent != nullptr)
        {
            parent->children.emplace_back(this);
        }
    }

    bool isChild(Block *block)
    {
        for (size_t i = 0; i < children.size(); i++)
        {
            if (children[i] == block)
                return true;
            else if (children[i]->isChild(block))
                return true;
        }
        return false;
    }

    virtual Type *get(Block *from, const std::string &name, bool allowParentScope=true)
    {
        if (members.find(name) != members.end()) {
            if (modifiers.find(name) != modifiers.end())
            {
                auto mods = modifiers[name];
                if (mods.accessModifier == AccessModifiers::Private || mods.accessModifier == AccessModifiers::Protected && !(isChild(from) && from == this))
                    throw RuntimeException("member \"" + name + "\" is private/protected within this context");
            }
            return members[name];
        }
        else if (parent != nullptr && allowParentScope)
            return parent->get(from, name);
        else
            throw RuntimeException("member \"" + name + "\" not found in this scope");
    }

    virtual void addMember(Block *from, const std::string &name, Type *member)
    {
        if (modifiers.find(name) != modifiers.end())
        {
            auto mods = modifiers[name];
            if (mods.isConst)
                throw RuntimeException("cannot assign to const member \"" + name + "\"");
            else if (mods.accessModifier == AccessModifiers::Private || mods.accessModifier == AccessModifiers::Protected && !(isChild(from) || from == this))
                throw RuntimeException("member \"" + name + "\" is private/protected within this context");
        }
        else
        {
            modifiers[name] = Modifiers();
        }

        members[name] = member;
    }

    virtual void addMember(Block *from, const std::string &name, Modifiers mods, Type *member)
    {
        if (modifiers.find(name) != modifiers.end())
        {
            throw RuntimeException("cannot specify modifiers for member \"" + name + "\": modifiers already set");
            auto mods = modifiers[name];
            if (mods.isConst)
                throw RuntimeException("cannot assign to const member \"" + name + "\"");
            if (mods.accessModifier == AccessModifiers::Private || mods.accessModifier == AccessModifiers::Protected && !isChild(from))
                throw RuntimeException("member \"" + name + "\" is private within this context");
        }

        members[name] = member;
        modifiers[name] = mods;
    }
};


#endif //S2S_BLOCK_H
