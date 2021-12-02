//
// Created by iashi on 26.11.2021.
//

#ifndef S2S_RUNTIME_H
#define S2S_RUNTIME_H


#include "../Types/Type.h"
#include "../Parser/Parser.h"
#include "../Types/IntType.h"
#include "../Types/FloatType.h"
#include "../Types/StringType.h"
#include "../Types/ArrayType.h"
#include "../Types/BoolType.h"
#include "../Types/Function.h"
#include "RuntimeException.h"
#include "Block.h"
#include "../Builtin/Functions/Print.h"
#include "../Builtin/Functions/Input.h"
#include "../Builtin/Functions/Types.h"
#include <map>
#include <string>
#include <utility>



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
        switch (node->type)
        {
            case ParserNode::NodeType::Assignment:
            {
                auto *n = (AssignmentNode *)node;
                SymbolNode *s = n->to;
                Type *value = runNode(n->what);
                currentBlock->variables[s->name] = value;
                return value;
            }
            case ParserNode::NodeType::Value:
            {
                return getValue(node);
            }
            case ParserNode::NodeType::Array:
            {
                auto *n = (ArrayNode *)node;
                auto *v = new ArrayType();
                for (auto & element : n->elements)
                {
                    v->addValue(runNode(element));
                }
                return v;
            }
            case ParserNode::NodeType::Symbol:
            {
                auto *n = (SymbolNode *)node;
                return currentBlock->getVariable(n->name);
            }
            case ParserNode::NodeType::Operation:
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
            case ParserNode::NodeType::Parentheses:
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
                currentBlock->functions[n->name] = new Function(currentBlock, n);
                return nullptr;
            }
            case ParserNode::FunctionCall:
            {
                auto n = (FunctionCallNode*)node;
                std::vector<Type*> args;
                for (auto & arg : n->args)
                {
                    args.emplace_back(runNode(arg));
                }
                return runFunction(currentBlock->getFunction(n->functionName), args);
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

        mainBlock->addFunction(new Print());
        mainBlock->addFunction(new Input());
        mainBlock->addFunction(new IntFunction());
        mainBlock->addFunction(new FloatFunction());
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
};


#endif //S2S_RUNTIME_H
