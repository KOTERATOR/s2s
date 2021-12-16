//
// Created by iashi on 16.12.2021.
//

#ifndef S2S_SYSTEM_H
#define S2S_SYSTEM_H


#include <Runtime/Builtin/Types/ClassType.h>
#include <Runtime/Builtin/Types/IntType.h>

class System : public ClassType {
public:
    System(Block *b) : ClassType(b)
    {
        addMember(this, "VERSION", Modifiers(Public, true, true), new IntType(2));
        addNativeMethod("PrintS2S", Modifiers(Public, true, true), NATIVE(System::PrintS2S));
    }

    Type *PrintS2S(Runtime *r, Args args, KWArgs kwargs, Type *handle) {
        std::cout << "SYSTEM S2S" << std::endl;
        std::cout << "VERSION: " << handle->get(this, "VERSION")->toString() << std::endl;
        return nullptr;
    }
};


#endif //S2S_SYSTEM_H
