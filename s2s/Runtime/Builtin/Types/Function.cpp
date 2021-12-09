//
// Created by iashi on 27.11.2021.
//

#include "Function.h"
#include "Runtime/Runtime.h"
#include "Parser/Nodes/ParserNode.h"
#include "Parser/Nodes/SymbolNode.h"

Type *Function::invoke(Runtime *r, std::vector<Type *> args) {
    for (int i = 0; i < this->args.size(); i++)
    {
        ParserNode* f_arg = this->args[i];
        if (f_arg->type == ParserNode::Symbol)
        {
            r->currentBlock->addMember(this, ((SymbolNode*)f_arg)->name, args[i]);
        }
        /*else if (f_arg->type == ParserNode::Assignment)
        {
            r->runNode(f_arg);
            r->currentBlock->
        }*/
    }
    return r->run(body);
}
