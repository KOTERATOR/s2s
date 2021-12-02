#include "number.h"
#include "array.h"
#include "float.h"
#include "string.h"

Value *FloatValue::plus(Value *another){
    VALUEOP(another,
            return new FloatValue(number + NUMBER(another)->number),
            return new FloatValue(number + FLOAT(another)->number),
            return new FloatValue(number),
            return new FloatValue(number))}

Value *FloatValue::minus(Value *another){
    VALUEOP(another,
            return new FloatValue(number - NUMBER(another)->number),
            return new FloatValue(number - FLOAT(another)->number),
            return new FloatValue(number),
            return new FloatValue(number))}

Value *FloatValue::mult(Value *another){
    VALUEOP(another,
            return new FloatValue(number * NUMBER(another)->number),
            return new FloatValue(number * FLOAT(another)->number),
            return new FloatValue(number),
            return new FloatValue(number))}

Value *FloatValue::div(Value *another)
{
    VALUEOP(another,
            return new FloatValue(number / NUMBER(another)->number),
            return new FloatValue(number / FLOAT(another)->number),
            return new FloatValue(number),
            return new FloatValue(number))
}