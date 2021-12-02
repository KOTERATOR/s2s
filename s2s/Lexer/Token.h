//
// Created by iashi on 26.11.2021.
//

#ifndef S2S_TOKEN_H
#define S2S_TOKEN_H

#include <string>
#include <utility>

struct Token
{
    enum TokensEnum
    {
        Symbol,
        SpecialChar,
        Operation,
        BitOperation,
        CompareOperation,
        String,
        Number
    };

    TokensEnum type;
    std::string value;

    Token() = default;

    Token(TokensEnum type, std::string value) : type(type), value(std::move(value))
    {
    }

    std::string toString() const
    {
        std::string typeStr;
        switch (type)
        {
            case Symbol:
                typeStr = "Symbol";
                break;
            case SpecialChar:
                typeStr = "Special Char";
                break;
            case Operation:
                typeStr = "Operation";
                break;
            case BitOperation:
                typeStr = "Bit Operation";
                break;
            case CompareOperation:
                typeStr = "Compare Operation";
                break;
            case String:
                typeStr = "String";
                break;
            case Number:
                typeStr = "Number";
                break;
        }
        return "(" + typeStr + ", \"" + value + "\")";
    }
};


#endif //S2S_TOKEN_H
