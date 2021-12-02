#pragma once

#include <string>

#define NUMBER(x) ((NumberValue *)x)
#define FLOAT(x) ((FloatValue *)x)
#define STRING(x) ((StringValue *)x)
#define ARRAY(x) ((ArrayValue *)x)

#define VALUEOP(v, x1, x2, x3, x4) \
    switch (v->type)               \
    {                              \
    case Type::Number:             \
        x1;                        \
        break;                     \
    case Type::Float:              \
        x2;                        \
        break;                     \
    case Type::String:             \
        x3;                        \
        break;                     \
    case Type::Array:              \
        x4;                        \
        break;                     \
    }

class Value
{
public:
    enum Type
    {
        Number,
        Float,
        String,
        Array,
        Custom
    };

    Type type;

    Value(Type t) : type(t) {}

    virtual std::string toString() { return std::string("Value::toString()"); }

    virtual Value *plus(Value *another) {}
    virtual Value *minus(Value *another) {}
    virtual Value *mult(Value *another) {}
    virtual Value *div(Value *another) {}

    virtual ~Value() {}
};