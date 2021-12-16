#include <iostream>
#include <fstream>

#include "s2s/Lexer/Lexer.h"
#include "s2s/Parser/Parser.h"
#include "s2s/Runtime/Runtime.h"
#include "s2s/Parser/Nodes/ParserNode.h"

int main(int argc, char **argv)
{
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
            std::vector<LexerLine> lines;
            while (!file.eof())
            {
                std::string str;
                std::getline(file, str);
                //std::cout << str << std::endl;
                try
                {
                    //std::cout << "input = " << input << std::endl;
                    LexerLine lexerLine = Lexer::tokenize(line, str);
                    for (int i = 0; i < lexerLine.tokens.size(); i++)
                    {
                        //std::cout << lexerLine.tokens[i].digestToString() << std::endl;
                    }
                    if (!lexerLine.tokens.empty())
                        lines.emplace_back(lexerLine);
                }
                catch (LexerException &e)
                {
                    std::cout << "Lexer error: " << e.what() << std::endl;
                }
                line++;
            }
            Parser p(lines);

            std::vector<ParserNode*> nodes = p.parse();
            for (auto &n : nodes)
            {
                if (n == nullptr)
                {
                    std::cout << "nullptr" << std::endl;
                }
                else
                {
                    std::cout << n->toString() << std::endl;
                }
            }
            Runtime r;
            try {
                r.run(nodes);
                //std::cout << r.blockInfo() << std::endl;
            }
            catch (RuntimeException &e)
            {
                std::cout << e.what() << std::endl;
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
                LexerLine lexerLine = Lexer::tokenize(line, input);
                for (int i = 0; i < lexerLine.tokens.size(); i++)
                {
                    std::cout << lexerLine.tokens[i].toString() << std::endl;
                }
                Parser p({lexerLine});
                std::vector<ParserNode *> n = p.parse();
                std::cout << n[0]->toString() << std::endl;
                Type *result = r.run(n[0]);
                std::cout << (result == nullptr ? "null" : result->toString()) << std::endl;
            }
            catch (LexerException &e)
            {
                std::cout << "Lexer error: " << e.what() << std::endl;
            }
        }
    }
}
