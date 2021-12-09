//
// Created by iashi on 26.11.2021.
//

#ifndef S2S_INTTYPE_H
#define S2S_INTTYPE_H

#include <cmath>
#include "Type.h"
#include "FloatType.h"
#include "BoolType.h"

class IntType : public Type
{
public:
    int value;
    IntType(int number) : Type(TypeEnum::Number)
    {
        this->value = number;
    }
    std::string toString() override { return std::to_string(value); }

    Type *plus(Type *) override;
    Type *minus(Type *) override;
    Type *mult(Type *) override;
    Type *div(Type *) override;

    int compare(Type *a) override
    {
        if (IS_NUMBER(a))
            return value - TO_NUMBER(a)->value;
        else if (IS_FLOAT(a))
            return ceil((float)value - TO_FLOAT(a)->value);
        else if (IS_BOOL(a))
            return value == 0;

    }
};


#endif //S2S_INTTYPE_H
