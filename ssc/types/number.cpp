#include "number.h"
#include "array.h"
#include "float.h"
#include "string.h"

#include <iostream>

Value *NumberValue::plus(Value *another){
    VALUEOP(another,
            return new NumberValue(number + NUMBER(another)->number),
            return new FloatValue(float(number) + FLOAT(another)->number),
            return new NumberValue(number),
            return new NumberValue(number))}

Value *NumberValue::minus(Value *another){
    VALUEOP(another,
            return new NumberValue(number - NUMBER(another)->number),
            return new FloatValue(float(number) - FLOAT(another)->number),
            return new NumberValue(number),
            return new NumberValue(number))}

Value *NumberValue::mult(Value *another){
    VALUEOP(another,
            return new NumberValue(number * NUMBER(another)->number),
            return new FloatValue(float(number) * FLOAT(another)->number),
            return new NumberValue(number),
            return new NumberValue(number))}

Value *NumberValue::div(Value *another)
{
    VALUEOP(another,
            return new NumberValue(number / NUMBER(another)->number),
            return new FloatValue(float(number) / FLOAT(another)->number),
            return new NumberValue(number),
            return new NumberValue(number))
}