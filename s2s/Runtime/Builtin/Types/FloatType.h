//
// Created by iashi on 26.11.2021.
//

#ifndef S2S_FLOATTYPE_H
#define S2S_FLOATTYPE_H


#include "Type.h"

class FloatType : public Type
{
public:
    float value;
    FloatType(float number) : Type(TypeEnum::Float)
    {
        this->value = number;
    }
    std::string toString() override { return std::to_string(value); }

    Type *plus(Type *) override;
    Type *minus(Type *) override;
    Type *mult(Type *) override;
    Type *div(Type *) override;
};


#endif //S2S_FLOATTYPE_H
