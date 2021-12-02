#include "number.h"
#include "array.h"
#include "float.h"
#include "string.h"

Value *StringValue::plus(Value *another){
    VALUEOP(another,
            return new StringValue(str + std::to_string(NUMBER(another)->number)),
            return new StringValue(str + std::to_string(FLOAT(another)->number)),
            return new StringValue(str),
            return new StringValue(str))}

Value *StringValue::minus(Value *another){
    VALUEOP(another,
            return new StringValue(str),
            return new StringValue(str),
            return new StringValue(str),
            return new StringValue(str))}

Value *StringValue::mult(Value *another){
    VALUEOP(
        another,
        {
            std::string n;
            for (int i = 0; i < NUMBER(another)->number; i++)
            {
                n += str;
            }
            return new StringValue(n);
        },
        return new StringValue(str),
        return new StringValue(str),
        return new StringValue(str))}

Value *StringValue::div(Value *another)
{
    VALUEOP(another,
            return new StringValue(str),
            return new StringValue(str),
            return new StringValue(str),
            return new StringValue(str))
}