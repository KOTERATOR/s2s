//
// Created by iashi on 26.11.2021.
//

#ifndef S2S_LEXEREXCEPTION_H
#define S2S_LEXEREXCEPTION_H

#include <string>
#include <exception>

class LexerException : public std::exception
{
private:
    int line;
    int pos;
    std::string info;

public:
    LexerException(int line, int pos, const std::string &info) : line(line), pos(pos)
    {
        this->info = (std::to_string(line) + ":" + std::to_string(pos) + " => " + info);
    }

    const char *what() const noexcept override
    {
        return info.c_str();
    }
};


#endif //S2S_LEXEREXCEPTION_H
