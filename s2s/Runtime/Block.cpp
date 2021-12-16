//
// Created by iashi on 27.11.2021.
//

#include "Block.h"

#include "Runtime/Builtin/Types/Function.h"

std::string Block::blockInfo(int level) {
    std::stringstream s;
    std::string append = "";
    for (int i = 0; i < level; i++)
        append += " ";
    s << append << "Block at " << std::hex << (size_t) this << std::dec << std::endl;
    s << append << "members:" << std::endl;
    for (auto &m: members) {
        s << append << m.first << ": " << std::endl << append << "  " << m.second->blockInfo(level + 1) << std::endl;
    }
    s << append << "children blocks:" << std::endl;
    for (auto &c: children) {
        s << append << c->blockInfo(level + 1) << std::endl;
    }
    return s.str();
}
