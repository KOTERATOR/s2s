#pragma once

#include "type.h"
#include <vector>

class ArrayValue : public Value
{
public:
    std::vector<Value *> values;
    ArrayValue() : Value(Type::Array)
    {
    }
    ArrayValue(std::vector<Value *> values) : Value(Type::Array)
    {
        this->values = values;
    }
    void addValue(Value *value) { values.push_back(value); }

    std::string toString() override
    {
        std::string result = "[";
        size_t size = values.size();
        for (size_t i = 0; i < size; i++)
        {
            result += values[i]->toString();
            if (i < size - 1)
                result += ", ";
        }
        result += "]";
        return result;
    }

    Value *plus(Value *) override;
    Value *minus(Value *) override;
    Value *mult(Value *) override;
    Value *div(Value *) override;
};
