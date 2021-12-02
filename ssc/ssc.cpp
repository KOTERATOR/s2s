#include <iostream>
#include <fstream>
#include <string>

#include "lexer.h"
#include "parser.h"
#include "runtime.h"

int main()
{
    std::cout << "Hello World" << std::endl;
    return 0;
}

/*int main(int argc, char **argv)
{
    std::cout << "Hello World!" << std::endl;
    std::cout << "argc = " << argc << std::endl;

    for (int i = 0; i < argc; i++)
    {
        std::cout << "arg" << i << " = " << argv[i] << std::endl;
    }

    if (argc >= 2)
    {
        char *filename = argv[1];
        std::ifstream file;
        std::cout << "filename: " << filename << std::endl;
        file.open(filename);
        if (file.is_open())
        {
            int line = 1;
            while (!file.eof())
            {
                std::string str;
                std::getline(file, str);
                std::cout << str << std::endl;
                try
                {
                    //std::cout << "input = " << input << std::endl;
                    Lexer::LexerLine lexerLine = Lexer::tokenize(line, str);
                    for (int i = 0; i < lexerLine.tokens.size(); i++)
                    {
                        std::cout << lexerLine.tokens[i].toString() << std::endl;
                    }
                }
                catch (LexerException &e)
                {
                    std::cout << "Lexer error: " << e.what() << std::endl;
                }
                line++;
            }
        }
        else
        {
            std::cout << "cannot open file" << std::endl;
        }
    }
    else
    {
        // interactive console
        std::string input;
        int line = 1;
        Runtime r;
        while (true)
        {
            std::cout << ">>> ";
            std::getline(std::cin, input);
            try
            {
                //std::cout << "input = " << input << std::endl;
                Lexer::LexerLine lexerLine = Lexer::tokenize(line, input);
                for (int i = 0; i < lexerLine.tokens.size(); i++)
                {
                    //std::cout << lexerLine.tokens[i].toString() << std::endl;
                }
                Parser p;
                ParserNode *n =  p.parseLine(lexerLine);
                std::cout << n->toString() << std::endl;
                Value *result = r.run(n);
                std::cout << (result == nullptr ? "None" : result->toString()) << std::endl;
            }
            catch (LexerException &e)
            {
                std::cout << "Lexer error: " << e.what() << std::endl;
            }
        }
    }
}*/