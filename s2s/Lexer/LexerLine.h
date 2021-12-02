//
// Created by iashi on 26.11.2021.
//

#ifndef S2S_LEXERLINE_H
#define S2S_LEXERLINE_H

#include <utility>
#include <vector>
#include "Token.h"

struct LexerLine
{
    int level = 0;
    std::vector<Token> tokens;

    LexerLine() = default;

    LexerLine(const LexerLine &line) : level(line.level), tokens(line.tokens)
    {
    }

    LexerLine(int level, std::vector<Token> tokens) : level(level), tokens(std::move(tokens))
    {
    }
};


#endif //S2S_LEXERLINE_H
