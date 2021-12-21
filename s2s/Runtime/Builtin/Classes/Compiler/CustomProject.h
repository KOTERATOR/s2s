//
// Created by iashi on 21.12.2021.
//

#ifndef S2S_CUSTOMPROJECT_H
#define S2S_CUSTOMPROJECT_H

#include <Runtime/Builtin/Types/ClassType.h>
#include <Runtime/Builtin/Types/ObjectType.h>
#include <Runtime/Builtin/Types/ArrayType.h>
#include <Runtime/Builtin/Types/StringType.h>
#include <Runtime/Builtin/Types/BoolType.h>

#include <utility>

class CustomProjectHandle : public ObjectType {
    void sort(Runtime *r, const std::string &name);

public:
    std::map<std::string, std::vector<std::string>> targets;
    std::map<std::string, int> statuses;
    std::vector<std::string> order;

    CustomProjectHandle(ClassType *t) : ObjectType(t) {

    }

    std::vector<std::string> GetOrder(Runtime *r, const std::string &name) {
        statuses.clear();
        order.clear();
        sort(r, name);
        return order;
    }
};

class CustomProject : public ClassType {
public:
    CustomProject(Block *b) : ClassType(b)
    {
        addNativeMethod("Constructor", Modifiers(Public, true, false), NATIVE(CustomProject::Constructor));
        addNativeMethod("AddTarget", Modifiers(Public, true, false), NATIVE(CustomProject::AddTarget));
        addNativeMethod("Run", Modifiers(Public, true, false), NATIVE(CustomProject::Run));
    }

    ObjectType *createInstance(Runtime *r, Args args, KWArgs kwargs) override {
        auto h = new CustomProjectHandle(this);
        h->Construct(r, args, kwargs);
        return h;
    }

    NATIVE_FUNCTION(Constructor) {
        addMember(handle, "action", args[0]);
        addMember(handle, "verify", args[1]);
        return handle;
    }

    NATIVE_FUNCTION(AddTarget) {
        if (IS_STRING(args[0]) && IS_ARRAY(args[1]))
        {
            auto h = (CustomProjectHandle*)handle;
            std::vector<std::string> deps;
            for (auto &d : TO_ARRAY(args[1])->values)
                deps.emplace_back(TO_NATIVE_STRING(d));
            h->targets[TO_NATIVE_STRING(args[0])] = deps;
        }
        return handle;
    }

    NATIVE_FUNCTION(Run) {
        if (IS_STRING(args[0])) {
            auto name = TO_STRING(args[0]);
            auto h = (CustomProjectHandle*)handle;
            auto order = h->GetOrder(r, name->str);
            if (order.empty()) {
                std::cout << "Target \"" << name->str << "\" is up-to-date" << std::endl;
            }
            else {
                for (auto &t: order) {
                    Run(r, t);
                }
            }
        }
        return handle;
    }

    void Run(Runtime *r, const std::string &t);
};


#endif //S2S_CUSTOMPROJECT_H
