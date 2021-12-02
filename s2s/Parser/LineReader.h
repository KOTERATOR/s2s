//
// Created by iashi on 29.11.2021.
//

#ifndef S2S_LINEREADER_H
#define S2S_LINEREADER_H

#include "../Lexer/LexerLine.h"
#include "Reader.h"

class LineReader : public Reader<Token> {
private:
    int level;
public:
    LineReader(const LexerLine &line) : Reader(line.tokens), level(line.level)
    {

    }

    int getLevel() const
    {
        return level;
    }
};


#endif //S2S_LINEREADER_H
