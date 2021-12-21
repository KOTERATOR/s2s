//
// Created by iashi on 04.12.2021.
//

#include "ObjectType.h"
#include <Runtime/Runtime.h>

Type *ObjectType::callMethod(Runtime *r, const std::string &name, Args args, KWArgs kwargs) {
    return r->runFunction(dynamic_cast<class Function *>(get(this, name, false)), args, kwargs, this);
}
