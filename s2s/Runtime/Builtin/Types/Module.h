//
// Created by iashi on 15.12.2021.
//

#ifndef S2S_MODULE_H
#define S2S_MODULE_H


#include "Type.h"

class Module : Type {
private:
    std::string name;
public:
    Module(const std::string &name) : Type(TypeEnum::Module) {
        this->name = name;
    }
};


#endif //S2S_MODULE_H
