//
// Created by iashi on 04.12.2021.
//

#include "ClassType.h"

#include <utility>
#include "Runtime/Runtime.h"

Block::Modifiers getClassModifiers(ModifiersNode* mn)
{
    auto mods = Runtime::getModifiers(mn);
    if (mods.accessModifier == Block::Default)
        mods.accessModifier = Block::Private;
    return mods;
}

ClassType::ClassType(Runtime *r, Block *parentBlock, ClassNode *classNode) : Type(TypeEnum::Class, parentBlock)
{
    this->name = classNode->name;
    std::cout << classNode->body.size() << std::endl;
    for (auto &b : classNode->body)
    {
        if (b->type == ParserNode::Function)
        {
            auto f = (FunctionNode*)b;
            auto mods = getClassModifiers(f->modifiers);
            auto f_type = mods.isStatic ? Function::Static : Function::Member;
            Block::addMember(this, f->name, mods, new class Function((Block*)this, f, f_type));
        }
        else if (b->type == ParserNode::Assignment)
        {
            auto a = (AssignmentNode*)b;
            if (a->to->type == ParserNode::Symbol)
            {
                auto s = (SymbolNode*)a->to;
                auto mods = getClassModifiers(s->modifiers);
                if (mods.isStatic) {
                    std::cout << "Added static variable - " << s->name << std::endl;
                    Block::addMember(this, s->name, mods, r->run(a->what, (Block *) this));
                }
                else
                    instanceVars.emplace_back(a);
            }
            else
            {
                throw RuntimeException("cannot assign to non-symbol");
            }
        }
        else if (b->type == ParserNode::Class)
        {
            auto cls = (ClassNode*)b;
            auto mods = getClassModifiers(cls->modifiers);
            auto innerClass = (ClassType*)r->run(cls, (Block*)this);
            if (members.find(innerClass->name) == members.end())
                Block::addMember(this, cls->name, mods, innerClass);
        }
        else
        {
            throw RuntimeException("invalid member in class " + name);
        }
    }
    parentBlock->addMember(this, name, this);
}

Type *&ClassType::get(Block *from, const std::string &name, bool allowParentScope) {
    if (modifiers.find(name) != modifiers.end())
    {
        auto mods = modifiers[name];
        if (!mods.isStatic)
            throw RuntimeException("member \"" + name + "\" of class \"" + this->name + "\" is not static");
    }
    return Block::get(from, name, allowParentScope);
}

Type *&ClassType::getMember(Block *from, const std::string &name) {
    return Block::get(from, name, false);
}

void ClassType::addMember(Block *from, const std::string &name, Type *member) {
    Block::addMember(from, name, member);
    modifiers[name].isStatic = true;
}

void ClassType::addMember(Block *from, const std::string &name, Block::Modifiers mods, Type *member) {
    Block::addMember(from, name, mods, member);
    modifiers[name].isStatic = true;
}

ObjectType *ClassType::createInstance(Runtime *r, Args args, KWArgs kwargs) {
    auto obj = new ObjectType(this);
    std::cout << "Create Instance of " << name << std::endl;
    for (auto &v : instanceVars)
    {
        if (v->to->type == ParserNode::Symbol)
        {
            auto s = (SymbolNode*)v->to;
            auto mods = getClassModifiers(s->modifiers);
            obj->addMember(this, s->name, mods, r->run(v->what, (Block*)this));
            std::cout << "\tadded member " << s->name << ": " << obj->members[s->name] << std::endl;
        }
        else
        {
            throw RuntimeException("cannot assign to non-symbol");
        }
    }
    obj->Construct(r, std::move(args), std::move(kwargs));
    return obj;
}

