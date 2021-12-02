#pragma once

#include "parser.h"
#include <map>

#include "types/type.h"
#include "types/number.h"
#include "types/float.h"
#include "types/string.h"
#include "types/array.h"

class Block
{
public:
    Block *parent = nullptr;
};

class NumberValue;
class FloatValue;
class StringValue;
class ArrayValue;

class Runtime
{
private:
    std::map<std::string, Value *> variables;

    Value *getValue(ParserNode *node)
    {
        ValueNode *n = (ValueNode *)node;
        switch (n->value.type)
        {
        case Lexer::TokensEnum::Number:
        {
            if (n->value.value.find(".") != std::string::npos)
            {
                return new FloatValue(std::atof(n->value.value.c_str()));
            }
            else
            {
                return new NumberValue(std::atoi(n->value.value.c_str()));
            }
            break;
        }
        case Lexer::TokensEnum::String:
            return new StringValue(n->value.value.c_str());
        default:
            // throw
            break;
        }
    }

    Value *runNode(ParserNode *node)
    {
        switch (node->type)
        {
        case ParserNode::NodeType::Assignment:
        {
            AssignmentNode *n = (AssignmentNode *)node;
            SymbolNode *s = n->to;
            Value *value = runNode(n->what);
            variables[s->name] = value;
            return value;
        }
        case ParserNode::NodeType::Value:
        {
            return getValue(node);
        }
        case ParserNode::NodeType::Array:
        {
            ArrayNode *n = (ArrayNode *)node;
            ArrayValue *v = new ArrayValue();
            for (int i = 0; i < n->elements.size(); i++)
            {
                v->addValue(runNode(n->elements[i]));
            }
            return v;
        }
        case ParserNode::NodeType::Symbol:
        {
            SymbolNode *n = (SymbolNode *)node;
            if (variables.find(n->name) != variables.end())
            {
                return variables[n->name];
            }
        }
        case ParserNode::NodeType::Operation:
        {
            OperationNode *n = (OperationNode*)node;
            auto op = n->op;
            Value *v1 = runNode(n->op1);
            Value *v2 = runNode(n->op2);
            if (op == "+")
                return v1->plus(v2);
            else if (op == "-")
                return v1->minus(v2);
            else if (op == "*")
                return v1->mult(v2);
            else if (op == "/")
                return v1->div(v2);
        }
        default:
            break;
        }
    }

public:
    Value *run(ParserNode *node)
    {
        return runNode(node);
    }
};