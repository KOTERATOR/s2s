//
// Created by iashi on 26.11.2021.
//

#ifndef S2S_RUNTIME_H
#define S2S_RUNTIME_H


#include "Builtin/Types/Type.h"
#include "Parser/Parser.h"
#include "Builtin/Types/IntType.h"
#include "Builtin/Types/FloatType.h"
#include "Builtin/Types/StringType.h"
#include "Builtin/Types/ArrayType.h"
#include "Builtin/Types/BoolType.h"
#include "Builtin/Types/Function.h"
#include "RuntimeException.h"
#include "Block.h"
#include "Builtin/Functions/Print.h"
#include "Builtin/Functions/Input.h"
#include "Builtin/Functions/Types.h"
#include "Parser/Nodes/ParserNode.h"

#include <map>
#include <string>
#include <utility>
#include <Runtime/Builtin/Types/ClassType.h>
#include <Runtime/Builtin/Types/ObjectType.h>
#include <Runtime/Builtin/Classes/System.h>
#include <Runtime/Builtin/Classes/Process.h>
#include <Runtime/Builtin/Classes/Hash.h>
#include <Runtime/Builtin/Classes/Compiler/Project.h>
#include <Runtime/Builtin/Classes/Compiler/CustomProject.h>
#include <Runtime/Builtin/Classes/Path.h>
#include <Runtime/Builtin/Classes/File.h>


class NumberValue;
class FloatValue;
class StringValue;
class ArrayValue;

class Runtime
{
private:
    friend class Function;
    Block *mainBlock;
    Block *currentBlock;

    static Type *getValue(ParserNode *node)
    {
        auto *n = (ValueNode *)node;
        switch (n->value.type)
        {
            case Token::Number:
            {
                if (n->value.value.find('.') != std::string::npos)
                {
                    return new FloatType(std::atof(n->value.value.c_str()));
                }
                else
                {
                    return new IntType(std::atoi(n->value.value.c_str()));
                }
                break;
            }
            case Token::String:
                return new StringType(n->value.value);
            case Token::Symbol:
                return new BoolType(n->value.value == "true");
            default:
                // throw
                return nullptr;
                break;
        }
    }

    Type *getMember(ParserNode *m, Block *b = nullptr, Block *from = nullptr)
    {
        if (b == nullptr)
            b = currentBlock;
        if (from == nullptr)
            from = currentBlock;

        if (m->type == ParserNode::MemberAccess)
        {
            auto ma = (MemberAccessNode*)m;
            auto member = getMember(ma->object, b, from);
            return getMember(ma->member, member, from);
        }
        else
        {
            return run(m, b, from);
        }
    }

    void setMemberValue(ParserNode *m, Type* value, Block *b = nullptr, Block *from = nullptr)
    {
        if (b == nullptr)
            b = currentBlock;

        if (from == nullptr)
            from = currentBlock;

        if (m->type == ParserNode::Symbol)
        {
            auto symbol = (SymbolNode*)m;
            if (symbol->modifiers != nullptr)
                b->addMember(from, symbol->name, getModifiers(symbol->modifiers), value);
            else
                b->addMember(from, symbol->name, value);
        }
        else if (m->type == ParserNode::GetOperation)
        {
            auto get = (GetNode*)m;
            auto v = run(get->what, b);
            auto i = run(get->value, from);
            v->op_getitem(i) = value;
        }
        else if (m->type == ParserNode::MemberAccess)
        {
            auto ma = (MemberAccessNode*)m;
            auto member = getMember(ma->object, b, from);
            return setMemberValue(ma->member, value, member, from);
        }
    }

    Type *runNode(ParserNode *node)
    {
        return runNode(node, currentBlock);
    }

    Type *runNode(ParserNode *node, Block *from)
    {
        switch (node->type)
        {
            case ParserNode::Assignment:
            {
                auto *n = (AssignmentNode *)node;
                ParserNode *s = n->to;
                Type *value = runNode(n->what);

                if (s->type == ParserNode::Symbol)
                {
                    auto symbol = (SymbolNode*)s;
                    if (symbol->modifiers != nullptr)
                        currentBlock->addMember(from, symbol->name, getModifiers(symbol->modifiers), value);
                    else
                        currentBlock->addMember(from, symbol->name, value);
                }
                else if (s->type == ParserNode::GetOperation)
                {
                    auto get = (GetNode*)s;
                    auto v = runNode(get->what);
                    auto i = runNode(get->value);
                    v->op_getitem(i) = value;
                }
                else if (s->type == ParserNode::MemberAccess)
                {
                    setMemberValue((MemberAccessNode*)s, value, currentBlock, from);
                }
                else
                {
                    throw RuntimeException("cannot assign to this");
                }

                return value;
            }
            case ParserNode::Value:
            {
                return getValue(node);
            }
            case ParserNode::Array:
            {
                auto *n = (ArrayNode *)node;
                auto *v = new ArrayType();
                for (auto & element : n->elements)
                {
                    v->addValue(runNode(element));
                }
                return v;
            }
            case ParserNode::Symbol:
            {
                auto *n = (SymbolNode *)node;
                /*std::string modifiersStr = std::to_string((int)mods.accessModifier);
                if (mods.isConst)
                {
                    modifiersStr += ", const";
                }
                if (mods.isStatic)
                {
                    modifiersStr += ", static";
                }
                std::cout << "Variable " << n->name << ", modifiers - " << modifiersStr << std::endl;*/
                return currentBlock->get(from, n->name);
            }
            case ParserNode::Operation:
            {
                auto *n = (OperationNode*)node;
                auto op = n->op;
                Type *v1 = runNode(n->op1);
                Type *v2 = runNode(n->op2);
                if (op == "+")
                    return v1->plus(v2);
                else if (op == "-")
                    return v1->minus(v2);
                else if (op == "*")
                    return v1->mult(v2);
                else if (op == "/")
                    return v1->div(v2);
            }
            case ParserNode::Parentheses:
            {
                return runNode(((ParenthesesNode*)node)->inner);
            }
            case ParserNode::CompareOperation:
            {
                auto *n = (CompareNode*)node;
                auto op = n->op;
                Type *v1 = runNode(n->op1);
                Type *v2 = runNode(n->op2);

                bool value = false;
                switch (op) {
                    case CompareNode::EQUALS:
                        value = v1->op_equals(v2);
                        break;
                    case CompareNode::NOT_EQUALS:
                        value = v1->op_not_equals(v2);
                        break;
                    case CompareNode::LT:
                        value = v1->op_lt(v2);
                        break;
                    case CompareNode::LTE:
                        value = v1->op_lte(v2);
                        break;
                    case CompareNode::GT:
                        value = v1->op_gt(v2);
                        break;
                    case CompareNode::GTE:
                        value = v1->op_gte(v2);
                        break;
                }
                return new BoolType(value);
            }
            case ParserNode::Function:
            {
                auto n = (FunctionNode*)node;
                auto f = new Function(currentBlock, n);
                if (n->modifiers != nullptr)
                    currentBlock->addMember(from, n->name, getModifiers(n->modifiers), f);
                else
                    currentBlock->addMember(from, n->name, f);
                return nullptr;
            }
            case ParserNode::FunctionCall:
            {
                auto n = (FunctionCallNode*)node;
                //std::cout <<"Function Call: " << n->function->toString() << std::endl;
                auto f = getMember(n->function, currentBlock, from);

                std::vector<Type*> args;
                KWArgs kwargs;
                Type *handle = nullptr;
                if (currentBlock->isTypeBlock)
                {
                    auto fromType = ((Type*)currentBlock);
                    auto ff = (Function*)f;
                    if (ff->functionType == Function::Member)
                    {
                        if (fromType->type == Type::Class)
                            throw RuntimeException("cannot call member function from class context");
                        else if (fromType->type != Type::Function)
                            handle = fromType;
                    }
                    else if (ff->functionType == Function::Static)
                    {
                        if (fromType->type == Type::Class)
                            handle = fromType;
                        else if (fromType->type != Type::Function)
                            handle = ((ObjectType*)fromType)->type;
                    }
                }

                for (auto & arg : n->args)
                {
                    if (arg->type == ParserNode::Assignment) {
                        auto a_arg = (AssignmentNode*)arg;
                        if (a_arg->to->type == ParserNode::Symbol) {
                            kwargs.args[((SymbolNode*)a_arg->to)->name] = runNode(a_arg->what);
                        }
                    }
                    else {
                        args.emplace_back(run(arg, from));
                    }
                }

                if (f->type == Type::Class)
                    return dynamic_cast<ClassType*>(f)->createInstance(this, args, kwargs);
                else if (f->type == Type::Function) {
                    return runFunction(dynamic_cast<Function *>(f), args, kwargs, handle);
                }
            }
            case ParserNode::Return:
            {
                auto n = (ReturnNode*)node;
                return runNode(n->what);
            }
            case ParserNode::If:
            {
                auto n = (IfNode*)node;
                auto v = runNode(n->condition);
                if (IS_BOOL(v) && TO_BOOL(v)->value)
                {
                    return run(n->body);
                }
                else
                {
                    for (auto &elseif : n->elseIfs)
                    {
                        auto ev = runNode(elseif->condition);
                        if (IS_BOOL(ev) && TO_BOOL(ev)->value) {
                            return run(elseif->body);
                        }
                    }

                    return run(n->elseBody);
                }
            }
            case ParserNode::GetOperation:
            {
                auto n = (GetNode*)node;
                auto v = runNode(n->what);
                auto i = runNode(n->value);
                return v->op_getitem(i);
            }
            case ParserNode::Class:
            {
                return new ClassType(this, currentBlock, (ClassNode*)node);
            }
            case ParserNode::MemberAccess:
            {
                auto member = (MemberAccessNode*)node;
                if (member->object->type == ParserNode::Symbol)
                {
                    auto objNode = (SymbolNode*)member->object;
                    auto obj = currentBlock->get(from, objNode->name, true);
                    if (obj->type != Type::Function)
                    {
                        Block* objBlock = nullptr;
                        if (obj->type == Type::Class)
                            objBlock = (Block*)(Type*)obj;
                        else
                            objBlock = (Block*)(Type*)obj;

                        for (const auto &m : objBlock->members) {
                            //std::cout << "\t" << m.first << ": " << m.second << std::endl;
                        }

                        return run(member->member, objBlock);
                    }
                    else
                    {
                        // TODO: throw
                    }
                }
                else
                {
                    return getMember(member);
                }
                //member->member->;
                //return run(member->object, b);
            }
            default:
                break;
        }
        return nullptr;
    }

public:
    Runtime()
    {
        mainBlock = new Block();
        currentBlock = mainBlock;

        mainBlock->addMember(mainBlock, "print", new Print());
        mainBlock->addMember(mainBlock, "input", new Input());
        mainBlock->addMember(mainBlock, "int", new IntFunction());
        mainBlock->addMember(mainBlock, "float",new FloatFunction());
        mainBlock->addMember(mainBlock, "System", new System(mainBlock));
        mainBlock->addMember(mainBlock, "Process", new Process(mainBlock));
        mainBlock->addMember(mainBlock, "Hash", new Hash(mainBlock));
        mainBlock->addMember(mainBlock, "Project", new Project(mainBlock));
        mainBlock->addMember(mainBlock, "CustomProject", new CustomProject(mainBlock));
        mainBlock->addMember(mainBlock, "Path", new Path(mainBlock));
        mainBlock->addMember(mainBlock, "File", new File(mainBlock));
    }

    Type *run(ParserNode *node, Block *block)
    {
        auto temp = currentBlock;
        currentBlock = block;
        auto r = runNode(node, temp);
        currentBlock = temp;
        return r;
    }

    Type *run(ParserNode *node, Block *block, Block *from)
    {
        auto temp = currentBlock;
        currentBlock = block;
        if (from == nullptr)
            from = temp;
        auto r = runNode(node, from);
        currentBlock = temp;
        return r;
    }

    Type *run(ParserNode *node)
    {
        return runNode(node);
    }

    Type *run(std::vector<ParserNode*> nodes)
    {
        //currentBlock = new Block(currentBlock);
        for (auto &n : nodes)
        {
            //std::cout << n->digestToString() << std::endl;
            if (n->type == ParserNode::Return)
                return runNode(n);
            else
                runNode(n);
        }
        return nullptr;
    }

    std::string blockInfo()
    {
        return mainBlock->blockInfo();
    }

    static Block::Modifiers getModifiers(ModifiersNode *node)
    {
        Block::Modifiers mods { Block::AccessModifiers::Default, false, false };
        if (node != nullptr)
        {
            for (auto &n : node->modifiers)
            {
                if (n == "public")
                    mods.accessModifier = Block::Public;
                else if (n == "protected")
                    mods.accessModifier = Block::Protected;
                else if (n == "private")
                    mods.accessModifier = Block::Private;
                else if (n == "static")
                    mods.isStatic = true;
                else if (n == "const")
                    mods.isConst = true;
            }
        }
        return mods;
    }

    Type *runFunction(Function *f, std::vector<Type *> args, KWArgs kwargs, Type *handle)
    {
        auto *functionBlock = new Block(handle == nullptr ? f->parentBlock : handle);
        Block *currentRuntimeBlock = currentBlock;
        currentBlock = functionBlock;
        Type *value = f->invoke(this, std::move(args), std::move(kwargs), handle);
        currentBlock = currentRuntimeBlock;
        return value;
    }
};


#endif //S2S_RUNTIME_H
