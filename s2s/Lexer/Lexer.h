//
// Created by iashi on 26.11.2021.
//

#ifndef S2S_LEXER_H
#define S2S_LEXER_H


#include <string>
#include <cstring>
#include <vector>

#include "LexerException.h"
#include "LexerLine.h"


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
                    bool operation = false;
                    if (charIn(c, "<>!="))
                    {
                        if (i + 1 < len)
                        {
                            char c1 = str[i + 1];
                            if (c1 == '=')
                            {
                                value += c1;
                                tokens.emplace_back(Token::CompareOperation, value);
                                operation = true;
                                i += 1;
                            }
                            else if (charIn(c1, "<>"))
                            {
                                if (c == c1)
                                {
                                    value += c1;
                                    tokens.emplace_back(Token::BitOperation, value);
                                    operation = true;
                                    i += 1;
                                }
                            }
                            else if (charIn(c, "<>"))
                            {
                                tokens.emplace_back(Token::CompareOperation, value);
                                operation = true;
                            }
                        }
                        else
                        {
                            if (charIn(c, "<>"))
                            {
                                tokens.emplace_back(Token::CompareOperation, value);
                                operation = true;
                            }
                        }
                    }

                    if (!operation)
                        tokens.emplace_back(Token::SpecialChar, value);
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
                        else if (r == '\\')
                        {
                            if (j + 1 < len)
                            {
                                char rs = str[j + 1];
                                switch (rs)
                                {
                                    case '\\':
                                        r = '\\';
                                        break;
                                    case 'n':
                                        r = '\n';
                                        break;
                                    case 't':
                                        r = '\t';
                                        break;
                                    case 'r':
                                        r = '\r';
                                        break;
                                    case '\"':
                                        r = '\"';
                                        break;
                                    case '\'':
                                        r = '\'';
                                        break;
                                    default:
                                        throw LexerException(line, j, "expected char after '\\'");
                                        break;
                                }
                                value += r;
                                j++;
                            }
                            else {
                                throw LexerException(line, j, "expected char after '\\'");
                            }
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
                    tokens.emplace_back(Token::String, value);
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
                    tokens.emplace_back(Token::Number, value);
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
                    tokens.emplace_back(Token::Symbol, value);
                }
                else if (charIn(c, "+-*/%"))
                {
                    std::string value;
                    value += c;
                    tokens.emplace_back(Token::Operation, value);
                }
                else if (charIn(c, "|&^"))
                {
                    std::string value;
                    value += c;
                    tokens.emplace_back(Token::BitOperation, value);
                }
                else
                {
                    throw LexerException(line, i, "unknown error");
                }
            }
        }
        return {level, tokens};
    }
};


#endif //S2S_LEXER_H
