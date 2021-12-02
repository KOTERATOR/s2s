#pragma once

#include <string>
#include <cstring>
#include <vector>
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

    const char *what() const throw()
    {
        return info.c_str();
    }
};

class Lexer
{
private:
    static bool charIn(char c, const char *str)
    {
        for (int i = 0; i < strlen(str); i++)
        {
            if (c == str[i])
                return true;
        }
        return false;
    }

    static bool charInRange(char c, char s, char e)
    {
        return c >= s && c <= e;
    }

    static bool charIsNum(char c) { return charInRange(c, '0', '9'); }
    static bool charInAlphabet(char c) { return charInRange(c, 'a', 'z') || charInRange(c, 'A', 'Z'); }

public:
    enum TokensEnum
    {
        Symbol,
        SpecialChar,
        Operation,
        String,
        Number
    };

    struct Token
    {
        TokensEnum type;
        std::string value;

        Token() {}

        Token(TokensEnum type, std::string value) : type(type), value(value)
        {
        }

        std::string toString()
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

    struct LexerLine
    {
        int level = 0;
        std::vector<Token> tokens;

        LexerLine() {}

        LexerLine(int level, const std::vector<Token> &tokens) : level(level), tokens(tokens)
        {
        }
    };

    static LexerLine tokenize(int line, std::string str)
    {
        std::vector<Token> tokens;
        bool started = false;
        int level = 0, whitespaces = 0;

        int len = str.length();
        for (int i = 0; i < len; i++)
        {
            char c = str[i];
            if (charIn(c, " \t\n"))
            {
                if (!started)
                {
                    if (c == ' ')
                    {
                        whitespaces++;
                        if (whitespaces % 4 == 0)
                            level++;
                    }
                    else if (c == '\t')
                    {
                        level++;
                    }
                }
                continue;
            }
            else
            {
                if (!started)
                {
                    started = true;
                    if (whitespaces % 4 != 0)
                        throw LexerException(line, i, "line not aligned");
                }
                if (charIn(c, "()[],.=:><!"))
                {
                    std::string value;
                    value += c;
                    tokens.push_back(Token(TokensEnum::SpecialChar, value));
                }
                else if (charIn(c, "\'\""))
                {
                    char delim = c;
                    bool closed = false;
                    std::string value;
                    int j = i + 1;
                    while (j < len)
                    {
                        char r = str[j];
                        if (r == delim)
                        {
                            closed = true;
                            break;
                        }
                        else
                        {
                            value += r;
                        }
                        j++;
                    }
                    i = j;

                    if (!closed)
                    {
                        throw LexerException(line, i, "String was not closed");
                    }
                    tokens.push_back(Token(TokensEnum::String, value));
                }
                else if (charIsNum(c))
                {
                    std::string value;
                    value += c;

                    bool point = false;
                    int j = i + 1;
                    while (j < len)
                    {
                        char r = str[j];
                        if (charIsNum(r))
                        {
                            value += r;
                        }
                        else if (r == '.')
                        {
                            if (!point)
                            {
                                point = true;
                                value += '.';
                            }
                            else
                            {
                                throw LexerException(line, j, "Second dot in float");
                            }
                        }
                        else
                        {
                            break;
                        }
                        j++;
                    }
                    i = j - 1;
                    tokens.push_back(Token(TokensEnum::Number, value));
                }
                else if (c == '_' || charInAlphabet(c))
                {
                    std::string value;
                    value += c;
                    int j = i + 1;
                    while (j < len)
                    {
                        char r = str[j];
                        if (r == '_' || charInAlphabet(r) || charIsNum(r))
                            value += r;
                        else
                            break;
                        j++;
                    }
                    i = j - 1;
                    tokens.push_back(Token(TokensEnum::Symbol, value));
                }
                else if (charIn(c, "+-*/%"))
                {
                    std::string value;
                    value += c;
                    tokens.push_back(Token(TokensEnum::Operation, value));
                }
                else
                {
                    throw LexerException(line, i, "unknown error");
                }
            }
        }
        return LexerLine(level, tokens);
    }
};