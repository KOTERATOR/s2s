//
// Created by iashi on 26.11.2021.
//

#include "IntType.h"
#include "ArrayType.h"
#include "FloatType.h"
#include "StringType.h"

Type *StringType::plus(Type *another){
    VALUEOP(another,
            return new StringType(str + std::to_string(TO_NUMBER(another)->value)),
            return new StringType(str + std::to_string(TO_FLOAT(another)->value)),
            return new StringType(str + TO_STRING(another)->str),
            return new StringType(str))}

Type *StringType::minus(Type *another){
    VALUEOP(another,
            return new StringType(str),
            return new StringType(str),
            return new StringType(str),
            return new StringType(str))}

Type *StringType::mult(Type *another){
    VALUEOP(
            another,
            {
                std::string n;
                for (int i = 0; i < TO_NUMBER(another)->value; i++)
                {
                    n += str;
                }
                return new StringType(n);
            },
            return new StringType(str),
            return new StringType(str),
            return new StringType(str))}

Type *StringType::div(Type *another)
{
    VALUEOP(another,
            return new StringType(str),
            return new StringType(str),
            return new StringType(str),
            return new StringType(str))
}
