//
// Created by iashi on 26.11.2021.
//

#include "IntType.h"
#include "ArrayType.h"
#include "FloatType.h"
#include "StringType.h"

Type *FloatType::plus(Type *another){
    VALUEOP(another,
            return new FloatType(value + TO_NUMBER(another)->value),
            return new FloatType(value + TO_FLOAT(another)->value),
            return new FloatType(value),
            return new FloatType(value))}

Type *FloatType::minus(Type *another){
    VALUEOP(another,
            return new FloatType(value - TO_NUMBER(another)->value),
            return new FloatType(value - TO_FLOAT(another)->value),
            return new FloatType(value),
            return new FloatType(value))}

Type *FloatType::mult(Type *another){
    VALUEOP(another,
            return new FloatType(value * TO_NUMBER(another)->value),
            return new FloatType(value * TO_FLOAT(another)->value),
            return new FloatType(value),
            return new FloatType(value))}

Type *FloatType::div(Type *another)
{
    VALUEOP(another,
            return new FloatType(value / TO_NUMBER(another)->value),
            return new FloatType(value / TO_FLOAT(another)->value),
            return new FloatType(value),
            return new FloatType(value))
}
