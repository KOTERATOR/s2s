//
// Created by iashi on 27.11.2021.
//

#include "Function.h"
#include "Runtime/Runtime.h"
#include "Parser/Nodes/ParserNode.h"
#include "Parser/Nodes/SymbolNode.h"

Type *Function::invoke(Runtime *r, Args args, KWArgs kwargs, Type *handle) {
    if (cls != nullptr && nativeFunction != nullptr)
        return (cls->*nativeFunction)(r, args, kwargs, handle);

    bool startedKWArgs = false;
    bool startedArgs = false;
    int argCounter = 0;

    if (handle != nullptr) {
        if (functionType == FunctionType::Member) {
            // TODO: const this
            r->currentBlock->addMember(this, "this", handle);
        } else if (functionType == FunctionType::Static) {
            r->currentBlock->addMember(this, "cls", handle);
        }
    }

    for (int i = 0; i < this->args.size(); i++)
    {
        ParserNode* f_arg = this->args[i];
        if (f_arg->type == ParserNode::Symbol)
        {
            if (startedKWArgs)
                throw RuntimeException("invalid function signature: arg after kwarg");

            auto f_s_arg = (SymbolNode*)f_arg;
            if (f_s_arg->name == "_args_")
            {
                if (!startedArgs)
                    startedArgs = true;

                std::vector<Type *> _args_;
                for (int j = i; j < args.size(); j++) {
                    argCounter++;
                    _args_.emplace_back(args[j]);
                }
                r->currentBlock->addMember(this, f_s_arg->name, new ArrayType(_args_));
            }
            else {
                if (startedArgs)
                    throw RuntimeException("only kwargs allowed after _args_");
                argCounter++;
                if (i >= args.size())
                    throw RuntimeException("function called with invalid count of arguments");
                r->currentBlock->addMember(this, f_s_arg->name, args[i]);
            }
        }
        else if (f_arg->type == ParserNode::Assignment)
        {
            if (!startedKWArgs)
                startedKWArgs = true;
            auto f_a_arg = (AssignmentNode*)f_arg;
            if (f_a_arg->to->type == ParserNode::Symbol)
            {
                auto to = (SymbolNode*)f_a_arg->to;
                if (kwargs.has(to->name)) {
                    r->currentBlock->addMember(this, to->name, kwargs[to->name]);
                } else {
                    r->currentBlock->addMember(this, to->name, r->run(f_a_arg->what));
                }
            }
        }
    }
    if (argCounter != args.size()) {
        throw RuntimeException("function called with invalid count of arguments");
    }
    return r->run(body);
}
