//
// Created by iashi on 26.11.2021.
//

#ifndef S2S_STRINGTYPE_H
#define S2S_STRINGTYPE_H


#include <utility>

#include "Type.h"

class StringType : public Type
{
public:
    std::string str;
    StringType(std::string str) : Type(TypeEnum::String)
    {
        this->str = std::move(str);
    }
    std::string toString() override { return /*(std::string("\"") + */ str /*+ std::string("\""))*/; }

    Type *plus(Type *) override;
    Type *minus(Type *) override;
    Type *mult(Type *) override;
    Type *div(Type *) override;

    int compare(Type *another) override;
};


#endif //S2S_STRINGTYPE_H
