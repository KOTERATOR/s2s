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
                break;
        }
    }

    Type *runFunction(Function *f, std::vector<Type *> args)
    {
        auto *functionBlock = new Block(f->parentBlock);
        Block *currentRuntimeBlock = currentBlock;
        currentBlock = functionBlock;
        Type *value = f->invoke(this, std::move(args));
        currentBlock = currentRuntimeBlock;
        return value;
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
                    auto member = (MemberAccessNode*)s;

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
                auto mods = currentBlock->modifiers[n->name];
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
                std::cout <<"Function Call: " << n->functionName << std::endl;
                auto f = currentBlock->get(from, n->functionName);

                if (f->type == Type::Class)
                    return dynamic_cast<ClassType*>(f)->createInstance(this);
                else if (f->type == Type::Function) {
                    std::vector<Type*> args;
                    if (currentBlock->isTypeBlock)
                    {
                        auto fromType = ((Type*)currentBlock);
                        if (fromType->type != Type::Class && fromType->type != Type::Function)
                        {
                            args.emplace_back(fromType);
                        }
                    }

                    for (auto & arg : n->args)
                    {
                        args.emplace_back(runNode(arg));
                    }
                    return runFunction(dynamic_cast<Function *>(f), args);
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
                        return run(member->member, objBlock);
                    }
                    else
                    {
                        // TODO: throw
                    }
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
    }

    Type *run(ParserNode *node, Block *block)
    {
        auto temp = currentBlock;
        currentBlock = block;
        auto r = runNode(node, temp);
        currentBlock = temp;
        return r;
    }

    Type *run(ParserNode *node)
    {
        return runNode(node);
    }

    Type *run(std::vector<ParserNode*> nodes)
    {
        currentBlock = new Block(currentBlock);
        for (auto &n : nodes)
        {
            //std::cout << n->toString() << std::endl;
            if (n->type == ParserNode::Return)
                return runNode(n);
            else
                runNode(n);
        }
        return nullptr;
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
};


#endif //S2S_RUNTIME_H
