//
// Created by iashi on 26.11.2021.
//

#ifndef S2S_ARRAYTYPE_H
#define S2S_ARRAYTYPE_H

#include "Type.h"
#include <vector>

class ArrayType : public Type
{
public:
    std::vector<Type *> values;
    ArrayType() : Type(TypeEnum::Array)
    {
    }
    ArrayType(std::vector<Type *> values) : Type(TypeEnum::Array)
    {
        this->values = values;
    }
    void addValue(Type *value) { values.push_back(value); }

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

    Type *plus(Type *) override;
    Type *minus(Type *) override;
    Type *mult(Type *) override;
    Type *div(Type *) override;

    Type *&op_getitem(Type *another) override
    {
        if (IS_NUMBER(another))
        {
            return values[TO_NUMBER(another)->value % values.size()];
        }
        throw;
    }
};


#endif //S2S_ARRAYTYPE_H
