//
// Created by iashi on 26.11.2021.
//

#include "IntType.h"
#include "ArrayType.h"
#include "FloatType.h"
#include "StringType.h"

Type *ArrayType::plus(Type *another)
{
    ArrayType *n = new ArrayType(values);
    VALUEOP(another,
            n->addValue(new IntType(TO_NUMBER(another)->value)),
            n->addValue(new FloatType(TO_FLOAT(another)->value)),
            n->addValue(new StringType(TO_STRING(another)->str)),
            n->addValue(new ArrayType(TO_ARRAY(another)->values)))

    return n;
}

Type *ArrayType::minus(Type *another)
{
    ArrayType *n = new ArrayType(values);
    return n;
}

Type *ArrayType::mult(Type *another)
{
    ArrayType *n = new ArrayType(values);
    return n;
}

Type *ArrayType::div(Type *another)
{
    ArrayType *n = new ArrayType(values);
    return n;
}