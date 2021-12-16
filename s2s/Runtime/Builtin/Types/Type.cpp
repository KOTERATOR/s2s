//
// Created by iashi on 10.12.2021.
//

#include "Type.h"
#include "ObjectType.h"

std::string Type::typeToString(Type *type) {
    switch (type->type)
    {
        case Number:
            return "Number";
        case Float:
            return "Float";
        case String:
            return "String";
        case Array:
            return "Array";
        case Bool:
            return "Bool";
        case Function:
            return "Function";
        case Class:
            return "Class";
        case Custom:
            // TODO: return custom type name
            return ((ObjectType*)type)->type->name;
        default:
            return "unknown";
    }
}

void Type::addNativeMethod(const std::string &name, Block::Modifiers mods, NativeFunction function) {
    auto t = mods.isStatic ? Function::Static : Function::Member;
    auto f = new class Function(name, this, function, t);
    addMember(this, name, mods, f);
}

