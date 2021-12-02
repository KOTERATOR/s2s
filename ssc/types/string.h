#pragma once

#include "type.h"

class StringValue : public Value
{
public:
    std::string str;
    StringValue(std::string str) : Value(Type::String)
    {
        this->str = str;
    }
    std::string toString() override { return (std::string("\"") + str + std::string("\"")); }

    Value *plus(Value *) override;
    Value *minus(Value *) override;
    Value *mult(Value *) override;
    Value *div(Value *) override;
};