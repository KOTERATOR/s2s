//
// Created by iashi on 29.11.2021.
//

#ifndef S2S_LINESREADER_H
#define S2S_LINESREADER_H

#include "../Lexer/LexerLine.h"
#include "LineReader.h"
#include "Reader.h"

class LinesReader : public Reader<LineReader>{
private:
    size_t linesCount = 0;
public:
    LinesReader(const std::vector<LexerLine> &lexerLines) : Reader<LineReader>()
    {
        for (auto &e : lexerLines)
        {
            elements.emplace_back(e);
        }
    }


};


#endif //S2S_LINESREADER_H
