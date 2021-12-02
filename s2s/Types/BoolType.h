//
// Created by iashi on 26.11.2021.
//

#ifndef S2S_BOOLTYPE_H
#define S2S_BOOLTYPE_H

#include "Type.h"

class BoolType : public Type {
public:
    bool value;
    BoolType(bool value) : Type(TypeEnum::Bool)
    {
        this->value = value;
    }

    bool op_equals(Type *a) override;

    std::string toString() override { return value ? "true" : "false"; }
};


#endif //S2S_BOOLTYPE_H
