#pragma once

#include "type.h"

class FloatValue : public Value
{
public:
    float number;
    FloatValue(float number) : Value(Type::Number)
    {
        this->number = number;
    }
    std::string toString() override { return std::to_string(number); }

    Value *plus(Value *) override;
    Value *minus(Value *) override;
    Value *mult(Value *) override;
    Value *div(Value *) override;
};