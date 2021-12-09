//
// Created by iashi on 26.11.2021.
//

#ifndef S2S_TYPE_H
#define S2S_TYPE_H


#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <Runtime/Block.h>

class IntType;
class FloatType;
class StringType;
class ArrayType;
class BoolType;

#define TO_NUMBER(x) ((IntType *)(x))
#define TO_FLOAT(x) ((FloatType *)(x))
#define TO_STRING(x) ((StringType *)(x))
#define TO_ARRAY(x) ((ArrayType *)(x))
#define TO_BOOL(x) ((BoolType *)(x))

#define IS_TYPE(x, type) ((x)->type == (type))
#define IS_NUMBER(x) IS_TYPE(x, Type::TypeEnum::Number)
#define IS_FLOAT(x) IS_TYPE(x, Type::TypeEnum::Float)
#define IS_STRING(x) IS_TYPE(x, Type::TypeEnum::String)
#define IS_ARRAY(x) IS_TYPE(x, Type::TypeEnum::Array)
#define IS_BOOL(x) IS_TYPE(x, Type::TypeEnum::Bool)

#define VALUEOP(v, x1, x2, x3, x4) \
    switch ((v)->type)               \
    {                              \
    case TypeEnum::Number:             \
        x1;                        \
        break;                     \
    case TypeEnum::Float:              \
        x2;                        \
        break;                     \
    case TypeEnum::String:             \
        x3;                        \
        break;                     \
    case TypeEnum::Array:              \
        x4;                        \
        break;                     \
    }

class Function;

class Type : public Block
{
public:
    enum TypeEnum
    {
        Number,
        Float,
        String,
        Array,
        Bool,
        Function,
        Class,
        Custom
    };

    //std::map<std::string, Method*> methods;

    TypeEnum type;

    Type(TypeEnum t, Block *parentBlock = nullptr) : Block(parentBlock), type(t) { Block::isTypeBlock = true; }

    static void* operator new(std::size_t count)
    {
        //std::cout << "Type new for size " << count << '\n';
        return ::operator new(count);
    }
    static void* operator new[](std::size_t count)
    {
        //std::cout << "Type new[] for size " << count << '\n';
        return ::operator new(count);
    }

    static std::string typeToString(Type *type)
    {
        switch (type->type)
        {
            case Number:
                return "Number";
            case Float:
                return "Float";
            case String:
                return "String";
            case Array:
                return "Array";
            case Bool:
                return "Bool";
            case Function:
                return "Function";
            case Class:
                return "Class";
            case Custom:
                // TODO: return custom type name
                return "Custom";
            default:
                return "unknown";
        }
    }

    static std::string typeAddressString(Type *type)
    {
        std::stringstream stream;
        stream << "0x" << std::setfill('0') << std::setw(sizeof(size_t)*2) << std::hex << type;
        return stream.str();
    }

    virtual std::string toString() { return "<object with type " + typeToString(this) + " at " + typeAddressString(this) + ">"; }
    virtual Type *copy() { return nullptr; }

    virtual Type *plus(Type *another) { return nullptr; }
    virtual Type *minus(Type *another) { return nullptr; }
    virtual Type *mult(Type *another) { return nullptr; }
    virtual Type *div(Type *another) { return nullptr; }

    virtual int compare(Type *another) { return 0; }

    virtual bool op_gt(Type *another) { return compare(another) > 0; }
    virtual bool op_gte(Type *another) { return compare(another) >= 0; }
    virtual bool op_lt(Type *another) { return compare(another) < 0; }
    virtual bool op_lte(Type *another) { return compare(another) <= 0; }
    virtual bool op_equals(Type *another) { return compare(another) == 0; }
    virtual bool op_not_equals(Type *another) { return op_equals(another) != 0; }
    virtual Type *&op_getitem(Type *another) { throw; }

    virtual ~Type() = default;
};


#endif //S2S_TYPE_H
