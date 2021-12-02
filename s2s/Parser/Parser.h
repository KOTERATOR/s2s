//
// Created by iashi on 26.11.2021.
//

#ifndef S2S_PARSER_H
#define S2S_PARSER_H


#include "../Lexer/Lexer.h"
#include "../Lexer/Token.h"
#include "../Lexer/LexerLine.h"
#include "LinesReader.h"
#include <iostream>
#include <utility>

class ParserNode {
public:
    enum NodeType {
        Function,
        FunctionCall,
        Task,
        TaskCall,
        If,
        Operation,
        CompareOperation,
        BitOperation,
        Parentheses,
        Symbol,
        Value,
        Array,
        Assignment,
        Return
    };

    NodeType type;

    ParserNode(NodeType type) : type(type) {
    }

    virtual ~ParserNode() {
    }

    virtual std::string toString() {
        return std::string("ParserNode::toString");
    }
};

class SymbolNode : public ParserNode {
public:
    std::string name;

    SymbolNode(std::string name) : ParserNode(NodeType::Symbol) {
        this->name = name;
    }

    std::string toString() override {
        return std::string("SymbolNode: name=") + name;
    }
};

class ValueNode : public ParserNode {
public:
    Token value;

    ValueNode(Token value) : ParserNode(NodeType::Value) {
        this->value = value;
    }

    std::string toString() override {
        return std::string("ValueNode: value=") + value.toString();
    }
};

class FunctionNode : public ParserNode {
public:
    std::string name;
    std::vector<ParserNode *> args;
    std::vector<ParserNode *> body;

    FunctionNode(std::string name, std::vector<ParserNode *> args, std::vector<ParserNode *> body) : ParserNode(
            NodeType::Function) {
        this->name = std::move(name);
        this->args = std::move(args);
        this->body = std::move(body);
    }

    std::string toString() override {
        std::string argsStr = "[";
        for (auto &a: args) {
            argsStr += a->toString() + ", ";
        }
        argsStr += "]";

        std::string bodyStr = "[";
        for (auto &a: body) {
            bodyStr += a->toString() + ", ";
        }
        bodyStr += "]";

        return std::string("Function ") + name + std::string(" args=") + argsStr + std::string(" body=") + bodyStr;
    }
};

class FunctionCallNode : public ParserNode {
public:
    std::string functionName;
    std::vector<ParserNode *> args;

    FunctionCallNode(std::string functionName, std::vector<ParserNode *> args) : ParserNode(NodeType::FunctionCall) {
        this->functionName = std::move(functionName);
        this->args = std::move(args);
    }

    std::string toString() override {
        std::string argsStr = "[";
        for (int i = 0; i < args.size(); i++) {
            argsStr += args[i]->toString();
            if (i < args.size() - 1)
                argsStr += ", ";
        }
        argsStr += "]";
        return std::string("FunctionCallNode: name=") + functionName + std::string(" args=") + argsStr;
    }
};

class IfNode : public ParserNode {
public:
    ParserNode *condition;
    std::vector<ParserNode *> body;
    std::vector<IfNode *> elseIfs;
    std::vector<ParserNode *> elseBody;

    IfNode(ParserNode *condition, std::vector<ParserNode *> body) : ParserNode(NodeType::If), condition(condition),
                                                                    body(std::move(body)) {}

    IfNode(ParserNode *condition, std::vector<ParserNode *> body, std::vector<ParserNode *> elseBody) :
            ParserNode(NodeType::If), condition(condition), body(std::move(body)), elseBody(std::move(elseBody)) {}

    IfNode(ParserNode *condition, std::vector<ParserNode *> body, std::vector<IfNode *> elseIfs,
           std::vector<ParserNode *> elseBody) : ParserNode(NodeType::If), condition(condition), body(std::move(body)),
                                                 elseIfs(std::move(elseIfs)), elseBody(std::move(elseBody)) {}
};

class ReturnNode : public ParserNode {
public:
    ParserNode *what;

    ReturnNode(ParserNode *what) : ParserNode(NodeType::Return) {
        this->what = what;
    }

    std::string toString() override {
        return std::string("Return Node: ") + what->toString();
    }
};

class AssignmentNode : public ParserNode {
public:
    SymbolNode *to;
    ParserNode *what;

    AssignmentNode(SymbolNode *to, ParserNode *what) : ParserNode(NodeType::Assignment) {
        this->to = to;
        this->what = what;
    }

    std::string toString() override {
        return std::string("AssignmentNode:\n\tto: ") + to->toString() + std::string("\n\twhat: ") + what->toString();
    }
};

class OperationNode : public ParserNode {
public:
    std::string op;
    ParserNode *op1, *op2;

    OperationNode(std::string op, ParserNode *op1, ParserNode *op2) : ParserNode(NodeType::Operation) {
        this->op = std::move(op);
        this->op1 = op1;
        this->op2 = op2;
    }

    OperationNode *insert(const std::string &nop, ParserNode *nop1) {
        if (op == "*" || op == "/") {
            if (nop == "+" || nop == "-") {
                return new OperationNode(nop, nop1, this);
            } else {
                if (this->op1->type == NodeType::Operation) {
                    this->op1 = ((OperationNode *) this->op1)->insert(nop, nop1);
                } else {
                    auto *n = new OperationNode(nop, nop1, this->op1);
                    this->op1 = n;
                }

                return this;
            }
        } else {
            if (nop == "*" || nop == "/") {
                if (this->op1->type == NodeType::Operation) {
                    this->op1 = ((OperationNode *) this->op1)->insert(nop, nop1);
                } else {
                    auto *n = new OperationNode(nop, nop1, this->op1);
                    this->op1 = n;
                }
                return this;
            } else {
                if (this->op1->type == NodeType::Operation) {
                    this->op1 = ((OperationNode *) this->op1)->insert(nop, nop1);
                } else {
                    auto *n = new OperationNode(nop, nop1, this->op1);
                    this->op1 = n;
                }
                return this;
            }
        }
    }

    std::string toString() override {
        return std::string("OperationNode: op = ") + op + std::string("{ op1: ") + op1->toString() +
               std::string("} { op2: ") + op2->toString() + std::string(" }");
    }
};

class CompareNode : public ParserNode {
public:
    enum CompareOperation {
        EQUALS,
        NOT_EQUALS,
        GT,
        GTE,
        LT,
        LTE
    };

    CompareOperation op;
    ParserNode *op1;
    ParserNode *op2;

    CompareNode(CompareOperation op, ParserNode *op1, ParserNode *op2) : ParserNode(NodeType::CompareOperation) {
        this->op = op;
        this->op1 = op1;
        this->op2 = op2;
    }

    std::string toString() override {
        return std::string("CompareNode: op = ") + std::to_string((int) op) + std::string(" op1: ") + op1->toString() +
               std::string(" op2: ") + op2->toString();
    }
};

class ParenthesesNode : public ParserNode {
public:
    ParserNode *inner;

    ParenthesesNode(ParserNode *inner) : ParserNode(NodeType::Parentheses) {
        this->inner = inner;
    }

    std::string toString() override {
        return std::string("ParenthesesNode: inner = ") + inner->toString();
    }
};

class ArrayNode : public ParserNode {
public:
    std::vector<ParserNode *> elements;

    ArrayNode(std::vector<ParserNode *> elements) : ParserNode(NodeType::Array) {
        this->elements = std::move(elements);
    }

    std::string toString() override {
        std::string result = std::string("ArrayNode: elements=[");
        for (int i = 0; i < elements.size(); i++)
            result += elements[i]->toString();
        result += "]";
        return result;
    }
};

class Parser {
private:
    LinesReader lines;

    ParserNode *parseSymbol(const std::string& symbol) {
        if (symbol == "fun") {
            Token nameToken = *lines.current()->next();
            if (nameToken.type != Token::Symbol) {
                // throw
            }
            std::string name = nameToken.value;

            Token parenthesisToken = *lines.current()->next();
            if (parenthesisToken.type != Token::SpecialChar && parenthesisToken.value != "(") {
                // throw
            }

            auto args = nextSequence(",", ")");
            std::vector<ParserNode *> body = parseBody(lines.current()->getLevel());
            return new FunctionNode(name, args, body);
        } else if (symbol == "return") {
            return new ReturnNode(next(nullptr));
        }
        else if (symbol == "if")
        {
            return parseIf();
        }
        else {
            return new SymbolNode(symbol);
        }
    }

    FunctionCallNode *parseFunctionCall(const std::string &name) {
        auto args = nextSequence(",", ")");
        return new FunctionCallNode(name, args);
    }

    IfNode *parseIf()
    {
        auto l = lines.current();
        auto condition = next(nullptr);
        auto body = parseBody(l->getLevel());

        l = lines.next();
        bool hasElse = false;

        std::vector<IfNode*> elseIfs;
        std::vector<ParserNode*> elseBody;
        while (l != nullptr) {
            auto t = l->next();

            if (t != nullptr) {
                if (t->value == "else") {
                    if (hasElse)
                    {
                        // throw
                        //throw ParserException("second else");
                    }
                    hasElse = true;
                    elseBody = parseBody(l->getLevel());
                } else if (t->value == "elif") {
                    if (hasElse)
                    {
                        // throw
                        //throw ParserException("elif after else");
                    }
                    auto elifCond = next(nullptr);
                    auto elifBody = parseBody(l->getLevel());
                    elseIfs.emplace_back(new IfNode(elifCond, elifBody));
                }
                else
                {
                    lines.prev();
                    break;
                }
            }
            else
            {
                lines.prev();
                break;
            }
            l = lines.next();
        }
        return new IfNode(condition, body, elseIfs, elseBody);
    }

    std::vector<ParserNode *> parseBody(int level) {
        std::vector<ParserNode *> body;
        LineReader *l = lines.next();
        while (l != nullptr && l->getLevel() - 1 == level) {
            body.emplace_back(next(nullptr));
            if (lines.lookNext()->getLevel() - 1 == level) {
                l = lines.next();
            } else {
                break;
            }
        }
        return body;
    }

    std::vector<ParserNode *> nextSequence(std::string sep, std::string end) {
        std::vector<ParserNode *> seq;

        for (int i = lines.current()->getIndex(); i < lines.current()->size(); i++) {
            Token *t = lines.current()->lookNext();
            if (t->value == sep) {
                lines.current()->next();
            } else if (t->value == end) {
                lines.current()->next();
                break;
            } else {
                // throw
            }

            seq.push_back(next(nullptr));
        }
        return seq;
    }

    ParserNode *nextTo(const std::string &end) {
        ParserNode *node = nullptr;

        for (int i = lines.current()->getIndex(); i < lines.current()->size(); i++) {
            Token *t = lines.current()->lookNext();
            if (t->value == end) {
                lines.current()->next();
                break;
            } else {
                //tokenIndex++;
                // throw
            }

            node = next(node);
        }
        return node;
    }

    ParserNode *next(ParserNode *prev) {
        Token *t = lines.current()->next();
        if (t == nullptr) {
            return prev;
        }
        Token token = *t;
        Token::TokensEnum type = token.type;
        std::string value = token.value;

        if (prev == nullptr) {
            switch (type) {
                case Token::Number:
                case Token::String:
                    return next(new ValueNode(token));
                case Token::Symbol:
                    if (value == "true" || value == "false")
                        return next(new ValueNode(token));
                    return next(parseSymbol(token.value)/*new SymbolNode(token.value)*/);
            }
        }

        if (type == Token::SpecialChar) {
            if (value == "=") {
                if (prev->type != ParserNode::NodeType::Symbol) {
                }
                ParserNode *nextNode = next(nullptr);
                return next(new AssignmentNode((SymbolNode *) prev, nextNode));
            } else if (value == "[") {
                return next(new ArrayNode(nextSequence(",", "]")));
            } else if (value == "(") {
                if (prev != nullptr && prev->type == ParserNode::Symbol) {
                    // function call
                    return next(parseFunctionCall(((SymbolNode *) prev)->name));
                }
                return next(new ParenthesesNode(nextTo(")")));
            }
        } else if (type == Token::Operation) {
            ParserNode *nextNode = next(nullptr);
            ParserNode *n;
            if (nextNode->type == ParserNode::NodeType::Operation) {
                n = ((OperationNode *) nextNode)->insert(value, prev);
            } else if (nextNode->type == ParserNode::CompareOperation) {
                auto c = ((CompareNode *) nextNode);
                n = new OperationNode(value, prev, c->op1);
                c->op1 = n;
                n = c;
            } else {
                n = new OperationNode(value, prev, nextNode);
            }
            return next(n);
        } else if (type == Token::CompareOperation) {
            CompareNode::CompareOperation op = CompareNode::EQUALS;
            if (value == "==")
                op = CompareNode::EQUALS;
            else if (value == "!=")
                op = CompareNode::NOT_EQUALS;
            else if (value == "<")
                op = CompareNode::LT;
            else if (value == "<=")
                op = CompareNode::LTE;
            else if (value == ">")
                op = CompareNode::GT;
            else if (value == ">=")
                op = CompareNode::GTE;
            return next(new CompareNode(op, prev, next(nullptr)));
        }
        lines.current()->prev();
        return prev;
    }

public:
    Parser(const std::vector<LexerLine> &lines) : lines(lines) {}

    std::vector<ParserNode *> parse() {
        std::vector<ParserNode *> nodes;
        LineReader *l;
        while ((l = lines.next()) != nullptr) {
            std::cout << "Line l - " << l->getLevel() << " "
                      << (l->lookNext() != nullptr ? l->lookNext()->value : "null") << std::endl;
            ParserNode *res = next(nullptr);
            if (res == nullptr)
                continue;
            nodes.emplace_back(res);
        }
        return nodes;
    }

    /*ParserNode *parseLine(const LexerLine &line)
    {
        int prevLineIndex = lineIndex;
        //std::cout << "parseLine: line.tokens.size() = " << line.tokens.size() << std::endl;
        this->currentLine = line;
        tokenIndex = -1;

        auto n = next(nullptr);
        if (prevLineIndex == lineIndex)
        {
            if (tokenIndex == line.tokens.size()-1)
            {
                lineIndex++;
            }
        }
        return n;
    }

    std::vector<ParserNode*> parseLines(std::vector<LexerLine> &lines)
    {
        lineIndex = -1;
        this->lines = lines;
        LexerLine *l = &lines[0];

        std::vector<ParserNode*> nodes;
        //std::cout << "parseLines: lines.size() = " << this->lines.size() << std::endl;
        while ((l = nextLine()) != nullptr)
        {
            ParserNode* res = parseLine(*l);
            if (res == nullptr)
                continue;
            nodes.emplace_back(res);
        }
        while (lineIndex < this->lines.size())
        {
            ParserNode* res = parseLine(lines[lineIndex]);
            if (res == nullptr)
                continue;
            nodes.emplace_back(res);
        }
        return nodes;
    }*/
};


#endif //S2S_PARSER_H
