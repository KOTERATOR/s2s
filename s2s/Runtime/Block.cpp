//
// Created by iashi on 27.11.2021.
//

#include "Block.h"

#include "../Types/Function.h"

void Block::addFunction(Function *f) {
    functions[f->name] = f;
}
