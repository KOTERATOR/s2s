//
// Created by iashi on 26.11.2021.
//

#include "IntType.h"
#include "ArrayType.h"
#include "FloatType.h"
#include "StringType.h"

Type *IntType::plus(Type *another){
    VALUEOP(another,
            return new IntType(value + TO_NUMBER(another)->value),
            return new FloatType(float(value) + TO_FLOAT(another)->value),
            return new IntType(value),
            return new IntType(value))}

Type *IntType::minus(Type *another){
    VALUEOP(another,
            return new IntType(value - TO_NUMBER(another)->value),
            return new FloatType(float(value) - TO_FLOAT(another)->value),
            return new IntType(value),
            return new IntType(value))}

Type *IntType::mult(Type *another){
    VALUEOP(another,
            return new IntType(value * TO_NUMBER(another)->value),
            return new FloatType(float(value) * TO_FLOAT(another)->value),
            return new IntType(value),
            return new IntType(value))}

Type *IntType::div(Type *another)
{
    VALUEOP(another,
            return new IntType(value / TO_NUMBER(another)->value),
            return new FloatType(float(value) / TO_FLOAT(another)->value),
            return new IntType(value),
            return new IntType(value))
}