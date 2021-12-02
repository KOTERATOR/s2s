#pragma once

#include "lexer.h"
#include <iostream>

class ParserNode
{
public:
    enum NodeType
    {
        Function,
        FunctionCall,
        Task,
        TaskCall,
        Operation,
        Parentheses,
        Symbol,
        Value,
        Array,
        Assignment
    };

    NodeType type;

    ParserNode(NodeType type) : type(type)
    {
    }

    virtual ~ParserNode()
    {
    }

    virtual std::string toString()
    {
        return std::string("ParserNode::toString");
    }
};

class SymbolNode : public ParserNode
{
public:
    std::string name;

    SymbolNode(std::string name) : ParserNode(NodeType::Symbol)
    {
        this->name = name;
    }

    std::string toString() override
    {
        return std::string("SymbolNode: name=") + name;
    }
};

class ValueNode : public ParserNode
{
public:
    Lexer::Token value;

    ValueNode(Lexer::Token value) : ParserNode(NodeType::Value)
    {
        this->value = value;
    }

    std::string toString() override
    {
        return std::string("ValueNode: value=") + value.toString();
    }
};

class FunctionNode : public ParserNode
{
public:
    std::string name;
    //std::vector<VariableNode> args;
    std::vector<ParserNode> body;
};

class AssignmentNode : public ParserNode
{
public:
    SymbolNode *to;
    ParserNode *what;

    AssignmentNode(SymbolNode *to, ParserNode *what) : ParserNode(NodeType::Assignment)
    {
        this->to = to;
        this->what = what;
    }

    std::string toString() override
    {
        return std::string("AssignmentNode:\n\tto: ") + to->toString() + std::string("\n\twhat: ") + what->toString();
    }
};

class OperationNode : public ParserNode
{
public:
    std::string op;
    ParserNode *op1, *op2;
    OperationNode(std::string op, ParserNode *op1, ParserNode *op2) : ParserNode(NodeType::Operation)
    {
        this->op = op;
        this->op1 = op1;
        this->op2 = op2;
    }

    OperationNode *insert(std::string nop, ParserNode *nop1)
    {
        std::cout << "OperataionNode::insert(nop=" << nop << ", nop1=" << nop1->toString() << ")" << std::endl;
        if (op == "*" || op == "/")
        {
            if (nop == "+" || nop == "-")
            {
                return new OperationNode(nop, nop1, this);
            }
            else
            {
                if (this->op1->type == NodeType::Operation)
                {
                    this->op1 = ((OperationNode *)this->op1)->insert(nop, nop1);
                }
                else
                {
                    OperationNode *n = new OperationNode(nop, nop1, this->op1);
                    this->op1 = n;
                }

                return this;
            }
        }
        else
        {
            if (nop == "*" || nop == "/")
            {
                if (this->op1->type == NodeType::Operation)
                {
                    this->op1 = ((OperationNode *)this->op1)->insert(nop, nop1);
                }
                else
                {
                    OperationNode *n = new OperationNode(nop, nop1, this->op1);
                    this->op1 = n;
                }
                return this;
            }
            else
            {
                if (this->op1->type == NodeType::Operation)
                {
                    this->op1 = ((OperationNode *)this->op1)->insert(nop, nop1);
                }
                else
                {
                    OperationNode *n = new OperationNode(nop, nop1, this->op1);
                    this->op1 = n;
                }
                return this;
            }
        }
    }

    std::string toString() override
    {
        return std::string("OperationNode: op = ") + op + std::string("{ op1: ") + op1->toString() + std::string("} { op2: ") + op2->toString() + std::string(" }");
    }
};

class ParenthesesNode : public ParserNode
{
public:
    ParserNode *inner;

    ParenthesesNode(ParserNode *inner) : ParserNode(NodeType::Parentheses)
    {
        this->inner = inner;
    }

    std::string toString() override
    {
        return std::string("ParenthesesNode: inner = ") + inner->toString();
    }
};

class ArrayNode : public ParserNode
{
public:
    std::vector<ParserNode *> elements;
    ArrayNode(std::vector<ParserNode *> elements) : ParserNode(NodeType::Array)
    {
        this->elements = elements;
    }

    std::string toString() override
    {
        std::string result = std::string("ArrayNode: elements=[");
        for (int i = 0; i < elements.size(); i++)
            result += elements[i]->toString();
        result += "]";
        return result;
    }
};

class Parser
{
private:
    std::vector<Lexer::LexerLine> lines;
    Lexer::LexerLine line;
    int currentLine = 0;
    int currentToken = 0;

    ParserNode *parseSymbol(std::string symbol)
    {
        //if (symbol == "task")
    }

    std::vector<ParserNode *> nextSequence(std::string sep, std::string end)
    {
        std::vector<ParserNode *> seq;

        for (int i = currentToken; i < line.tokens.size(); i++)
        {
            Lexer::Token t = line.tokens[currentToken];
            if (t.value == sep)
            {
                currentToken++;
            }
            else if (t.value == end)
            {
                currentToken++;
                break;
            }
            else
            {
                // throw
            }

            seq.push_back(next(nullptr));
        }
        return seq;
    }

    ParserNode * nextTo(std::string end)
    {
        ParserNode * node = nullptr;

        for (int i = currentToken; i < line.tokens.size(); i++)
        {
            Lexer::Token t = line.tokens[currentToken];
            std::cout << "Next to " << i << ": " << t.toString() << std::endl;
            if (t.value == end)
            {
                currentToken++;
                break;
            }
            else
            {
                currentToken++;
                // throw
            }

            node = next(node);
        }
        return node;
    }

    ParserNode *next(ParserNode *prev)
    {

        //Lexer::LexerLine line = lines[currentLine];

        if (currentToken >= line.tokens.size())
        {
            return prev;
        }

        Lexer::Token token = line.tokens[currentToken++];
        Lexer::TokensEnum type = token.type;
        std::string value = token.value;

        if (prev == nullptr)
        {
            switch (type)
            {
            case Lexer::Number:
            case Lexer::String:
                return next(new ValueNode(token));
            case Lexer::Symbol:
                return next(new SymbolNode(token.value));
            }
        }

        if (type == Lexer::SpecialChar)
        {
            if (value == "=")
            {
                if (prev->type != ParserNode::NodeType::Symbol)
                {
                }
                ParserNode *nextNode = next(nullptr);
                return next(new AssignmentNode((SymbolNode *)prev, nextNode));
            }
            else if (value == "[")
            {
                return next(new ArrayNode(nextSequence(",", "]")));
            }
            else if (value == "(")
            {
                return next(new ParenthesesNode(nextTo(")")));
            }
        }
        else if (type == Lexer::Operation)
        {

            /*if (value == "*" || value == "/")
            {
                std::cout << "opa" << std::endl;
                ParserNode *nextNode = next(nullptr);
                if (nextNode->type == ParserNode::NodeType::Operation)
                {
                    OperationNode *n = (OperationNode*)nextNode;
                    ParserNode *md = new OperationNode(value, prev, n->op1);
                    n->op1 = md;

                    return next(n);
                }
                return next(new OperationNode(value, prev, nextNode));
            }
            else if (value == "-" || value == "+")
            {
                std::cout << "opa+-" << std::endl;
                ParserNode *nextNode = next(nullptr);
                
                if (nextNode->type == ParserNode::NodeType::Operation)
                {
                    ParserNode *prevNode = nextNode, *temp = nextNode;
                    while (temp->type == ParserNode::NodeType::Operation)
                    {
                        OperationNode *n = (OperationNode*)temp;
                        prevNode = temp;
                        temp = n->op1;
                    }

                    OperationNode *n = (OperationNode*)prevNode;
                    
                    std::cout << "PREV="<<prev->toString() << std::endl;
                    std::cout << "N=" << n->toString() << std::endl;
                    OperationNode *md = new OperationNode(value, prev, n->op1);
                    n->op1 = md;
                    //ParserNode *nop1 = n->op1;
                    //n->op1 = md;
                    //md->op2 = nop1;
                    
                    //std::cout << "OPA+-" << n->toString() << std::endl;
                    return next(nextNode);
                }

                return next(new OperationNode(value, prev, nextNode));
            }
            else
            {*/
            ParserNode *nextNode = next(nullptr);
            ParserNode *n;
            if (nextNode->type == ParserNode::NodeType::Operation)
            {
                n = ((OperationNode *)nextNode)->insert(value, prev);
            }
            else
            {
                n = new OperationNode(value, prev, nextNode);
            }
            std::cout << n->toString() << std::endl;
            return next(n);
            //}
        }
        currentToken--;
        return prev;
    }

public:
    Parser() {}

    ParserNode *parseLine(Lexer::LexerLine line)
    {
        this->line = line;
        currentToken = 0;
        return next(nullptr);
    }
};