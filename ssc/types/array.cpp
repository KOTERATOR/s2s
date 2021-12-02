#include "number.h"
#include "array.h"
#include "float.h"
#include "string.h"

Value *ArrayValue::plus(Value *another)
{
    ArrayValue *n = new ArrayValue(values);
    VALUEOP(another,
            n->addValue(new NumberValue(NUMBER(another)->number)),
            n->addValue(new FloatValue(FLOAT(another)->number)),
            n->addValue(new StringValue(STRING(another)->str)),
            n->addValue(new ArrayValue(ARRAY(another)->values)))

    return n;
}

Value *ArrayValue::minus(Value *another)
{
    ArrayValue *n = new ArrayValue(values);
    return n;
}

Value *ArrayValue::mult(Value *another)
{
    ArrayValue *n = new ArrayValue(values);
    return n;
}

Value *ArrayValue::div(Value *another)
{
    ArrayValue *n = new ArrayValue(values);
    return n;
}