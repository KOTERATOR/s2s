//
// Created by iashi on 26.11.2021.
//

#ifndef S2S_PARSER_H
#define S2S_PARSER_H


#include "../Lexer/Lexer.h"
#include "../Lexer/Token.h"
#include "../Lexer/LexerLine.h"
#include "LinesReader.h"
#include "Nodes/ParserNode.h"
#include "Nodes/Nodes.h"
#include <iostream>
#include <utility>


class Parser {
private:
    LinesReader lines;

    bool isModifier(const std::string &s) {
        return s == "private" || s == "public" || s == "protected" || s == "static" || s == "const";
    }

    ParserNode *parseSymbol(const std::string &symbol) {
        if (symbol == "fun") {
            Token nameToken = *lines.current()->next();
            if (nameToken.type != Token::Symbol) {
                // TODO: throw
            }
            std::string name = nameToken.value;
            if (name == "operator") {
                name += parseOperatorOperation();
            }

            Token parenthesisToken = *lines.current()->next();
            if (parenthesisToken.type != Token::SpecialChar && parenthesisToken.value != "(") {
                // TODO: throw
            }

            auto args = nextSequence(",", ")");
            std::vector<ParserNode *> body = parseBody(lines.current()->getLevel());
            return new FunctionNode(name, args, body);
        } else if (symbol == "class") {
            return parseClass();
        } else if (symbol == "return") {
            return new ReturnNode(next(nullptr));
        } else if (symbol == "if") {
            return parseIf();
        } else if (symbol == "import") {
            return new ModuleImportNode(next(nullptr));
        } else if (symbol == "operator") {
            return new SymbolNode(symbol + parseOperatorOperation());
        } else if (isModifier(symbol)) {
                std::vector<std::string> modifiers;
                modifiers.emplace_back(symbol);
                while (lines.current()->lookNext() != nullptr && isModifier(lines.current()->lookNext()->value)) {
                    modifiers.emplace_back(lines.current()->next()->value);
                }
                return new ModifiersNode(modifiers);
        } else {
            return new SymbolNode(symbol);
        }
    }

    std::string parseOperatorOperation() {
        auto nextToken = lines.current()->next();
        if (nextToken != nullptr && (nextToken->type == Token::Operation || nextToken->type == Token::SpecialChar || nextToken->type == Token::CompareOperation || nextToken->type == Token::BitOperation)) {
            auto value = nextToken->value;
            if (value == "[")
            {
                auto close = lines.current()->next();
                if (close != nullptr && close->value == "]") {
                    return "[]";
                }
                else {
                    // TODO: throw
                }
            }
            else if (value == "(") {
                // TODO: throw
            }
            else {
                return value;
            }
        }
    }

    FunctionCallNode *parseFunctionCall(ParserNode *n) {
        auto args = nextSequence(",", ")");
        return new FunctionCallNode(n, args);
    }

    ClassNode *parseClass() {
        Token nameToken = *lines.current()->next();
        if (nameToken.type != Token::Symbol) {
            // TODO: throw
        }
        return new ClassNode(nameToken.value, parseBody(lines.current()->getLevel()));
    }

    IfNode *parseIf() {
        auto l = lines.current();
        auto condition = next(nullptr);
        auto body = parseBody(l->getLevel());

        l = lines.next();
        bool hasElse = false;

        std::vector<IfNode *> elseIfs;
        std::vector<ParserNode *> elseBody;
        while (l != nullptr) {
            auto t = l->next();

            if (t != nullptr) {
                if (t->value == "else") {
                    if (hasElse) {
                        // throw
                        //throw ParserException("second else");
                    }
                    hasElse = true;
                    elseBody = parseBody(l->getLevel());
                } else if (t->value == "elif") {
                    if (hasElse) {
                        // throw
                        //throw ParserException("elif after else");
                    }
                    auto elifCond = next(nullptr);
                    auto elifBody = parseBody(l->getLevel());
                    elseIfs.emplace_back(new IfNode(elifCond, elifBody));
                } else {
                    l->prev();
                    lines.prev();
                    break;
                }
            } else {
                l->prev();
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

        for (size_t i = lines.current()->getIndex(); i < lines.current()->size(); i++) {
            Token *t = lines.current()->lookNext();
            std::cout << "t->value = \"" << t->value << "\"" << std::endl;
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

    ParserNode *nextSquareBracket(ParserNode *prev)
    {
        if (prev != nullptr) {
            return new GetNode(prev, nextTo("]"));
        }
        return new ArrayNode(nextSequence(",", "]"));
    }

    ParserNode *nextParenthesis(ParserNode *prev)
    {
        if (prev != nullptr) {
            // function call
            return parseFunctionCall(prev);
        }
        return new ParenthesesNode(nextTo(")"));
    }

    ParserNode *nextNonOp(ParserNode *prev) {
        Token *t = lines.current()->next();
        if (t == nullptr)
            return prev;

        Token token = *t;
        Token::TokensEnum type = token.type;
        std::string value = token.value;

        if (prev == nullptr) {
            switch (type) {
                case Token::Number:
                case Token::String:
                    return nextNonOp(new ValueNode(token));
                case Token::Symbol:
                    if (value == "true" || value == "false")
                        return nextNonOp(new ValueNode(token));
                    return nextNonOp(parseSymbol(token.value));
            }
        }

        if (type == Token::SpecialChar)
        {
            if (value == "[")
            {
                return nextSquareBracket(prev);
            }
            else if (value == "(")
            {
                return nextParenthesis(prev);
            }
            else if (value == ".")
            {
                auto member = nextNonOp(prev);
                return nextNonOp(new MemberAccessNode(prev, member));
            }
        }
        else if (type == Token::Symbol)
        {
            return nextNonOp(parseSymbol(token.value));
        }

        lines.current()->prev();
        return prev;
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
                    return next(parseSymbol(token.value));
            }
        }

        if (type == Token::SpecialChar) {
            if (value == "=") {
                if (prev->type != ParserNode::NodeType::Symbol) {
                }
                ParserNode *nextNode = next(nullptr);
                return next(new AssignmentNode(prev, nextNode));
            } else if (value == "[") {
                if (prev != nullptr) {
                    return next(new GetNode(prev, nextTo("]")));
                }
                return next(new ArrayNode(nextSequence(",", "]")));
            } else if (value == "(") {
                if (prev != nullptr && prev->type == ParserNode::Symbol) {
                    // function call
                    return next(parseFunctionCall(prev));
                }
                return next(new ParenthesesNode(nextTo(")")));
            } else if (value == ".") {
                // member access
                if (prev != nullptr) {
                    auto member = nextNonOp(prev);
                    return next(new MemberAccessNode(prev, member));
                } else {
                    // TODO: throw
                }
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
        } else if (type == Token::Symbol) {
            auto nextNode = parseSymbol(value);
            if (prev->type == ParserNode::Modifiers) {
                auto mods = dynamic_cast<ModifiersNode *>(prev);
                if (nextNode->type == ParserNode::Class) {
                    dynamic_cast<ClassNode *>(nextNode)->modifiers = mods;
                } else if (nextNode->type == ParserNode::Function) {
                    dynamic_cast<FunctionNode *>(nextNode)->modifiers = mods;
                } else if (nextNode->type == ParserNode::Symbol) {
                    dynamic_cast<SymbolNode *>(nextNode)->modifiers = mods;
                } else {
                    std::cout << "Unexpected modifiers or symbol" << std::endl;
                    // TODO: throw
                }
                return next(nextNode);
            }
            else {
                std::cout << "Awaited modifiers" << std::endl;
                // TODO: throw
            }

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
