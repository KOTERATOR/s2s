//
// Created by iashi on 27.11.2021.
//

#ifndef S2S_INPUT_H
#define S2S_INPUT_H

#include "../../Types/Function.h"

class Input : public Function {
public:
    Input() : Function("input")
    {

    }

    Type *invoke(Runtime *r, std::vector<Type *> args) override
    {
        size_t argsLen = args.size();
        if (argsLen > 0) {
            for (auto &arg: args) {
                std::cout << arg->toString();
                if (argsLen > 1)
                    std::cout << " ";
            }
            //std::cout << std::endl;
        }
        std::string line;
        std::getline(std::cin, line);
        return new StringType(line);
    }
};

#endif //S2S_INPUT_H
