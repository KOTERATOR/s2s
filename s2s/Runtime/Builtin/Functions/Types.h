//
// Created by iashi on 27.11.2021.
//

#ifndef S2S_TYPES_H
#define S2S_TYPES_H

#include "../Types/AllTypes.h"

class TypeFunction : public Function
{
public:
    TypeFunction(const std::string &name) : Function(name) {}

    Type *invoke(Runtime *r, Args args, KWArgs kwargs, Type *handle) override
    {
        if (args.size() > 1)
            throw RuntimeException("can't call " + name + " with more than 1 argument");
        auto arg = args[0];
        return convert(arg);
    }

    virtual Type* convert(Type *arg) = 0;
};

class IntFunction : public TypeFunction
{
public:
    IntFunction() : TypeFunction("int")
    {

    }

    Type *convert(Type * arg) override
    {
        int value = 0;
        if (IS_NUMBER(arg))
        {
            value = TO_NUMBER(arg)->value;
        }
        if (IS_STRING(arg))
        {
            auto str = TO_STRING(arg);
            try {
                value = std::stoi(str->str);
            }
            catch (std::exception &e) {
                throw RuntimeException("can't convert string \"" + str->str + "\" to int");
            }
        }
        else if (IS_BOOL(arg))
        {
            value = TO_BOOL(arg)->value ? 1 : 0;
        }
        else if (IS_FLOAT(arg))
        {
            value = (int)(TO_FLOAT(arg)->value);
        }
        else
        {
            throw RuntimeException("invalid argument");
        }
        return new IntType(value);
    }
};

class FloatFunction : public TypeFunction
{
public:
    FloatFunction() : TypeFunction("float")
    {

    }

    Type *convert(Type * arg) override
    {
        float value = 0;
        if (IS_NUMBER(arg))
        {
            value = (float)(TO_NUMBER(arg)->value);
        }
        if (IS_STRING(arg))
        {
            auto str = TO_STRING(arg);
            try {
                value = std::stof(str->str);
            }
            catch (std::exception &e) {
                throw RuntimeException("can't convert string \"" + str->str + "\" to float");
            }
        }
        else if (IS_BOOL(arg))
        {
            value = TO_BOOL(arg)->value ? 1.0f : 0.0f;
        }
        else if (IS_FLOAT(arg))
        {
            value = TO_FLOAT(arg)->value;
        }
        else
        {
            throw RuntimeException("invalid argument");
        }
        return new FloatType(value);
    }
};

#endif //S2S_TYPES_H
