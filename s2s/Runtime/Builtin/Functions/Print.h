//
// Created by iashi on 27.11.2021.
//

#ifndef S2S_PRINT_H
#define S2S_PRINT_H

#include "../Types/Function.h"

class Print : public Function {
public:
    Print() : Function("print")
    {

    }

    Type *invoke(Runtime *r, std::vector<Type *> args) override
    {
        size_t argsLen = args.size();
        for (auto & arg : args)
        {
            std::cout << arg->toString();
            if (argsLen > 1)
                std::cout << " ";
        }
        std::cout << std::endl;
        return nullptr;
    }
};


#endif //S2S_PRINT_H
