//
// Created by iashi on 26.11.2021.
//

#include "BoolType.h"
#include "IntType.h"

bool BoolType::op_equals(Type *a) {
    if (IS_BOOL(a))
        return value == TO_BOOL(a)->value;
    else if (IS_NUMBER(a))
        return value == (TO_NUMBER(a)->value != 0);
    else
        return false;
}
