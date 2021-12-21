//
// Created by iashi on 21.12.2021.
//

#include "CustomProject.h"
#include <Runtime/Runtime.h>

void CustomProjectHandle::sort(Runtime *r, const std::string &name) {
    if (targets.find(name) != targets.end()) {
        if (statuses.find(name) != statuses.end()) {
            if (statuses[name] == 0) {
                throw RuntimeException("found cyclic dependency: " + name);
            } else if (statuses[name] == 2 || statuses[name] == 1) {
                return;
            }
        }
        statuses[name] = 0;
        bool allSatisfied = true;
        for (auto &d : targets[name])
        {
            sort(r, d);
            if (statuses[d] == 1)
                allSatisfied = false;
        }
        auto verify_result = r->runFunction(dynamic_cast<class Function *>(get(this, "verify", true)), {new StringType(name)}, KWArgs(), nullptr);
        auto status = TO_NATIVE_BOOL(verify_result);
        if (allSatisfied && status) {
            statuses[name] = 2;
        } else {
            statuses[name] = 1;
            order.emplace_back(name);
        }
    }
    else
    {
        throw RuntimeException("target with name \"" + name + "\" not found");
    }
}

void CustomProject::Run(Runtime *r, const std::string &t) {
    r->runFunction(dynamic_cast<class Function *>(get(this, "action", true)), {new StringType(t)}, KWArgs(), nullptr);
}
