//
// Created by iashi on 16.12.2021.
//

#ifndef S2S_PROCESS_H
#define S2S_PROCESS_H

#include <Runtime/Builtin/Types/ClassType.h>
#include <Runtime/Builtin/Types/ObjectType.h>
#include <Runtime/Builtin/Types/StringType.h>

#include <cstdio>

class Process : public ClassType {
private:
public:
    Process(Block *b) : ClassType(b) {
        addNativeMethod("Constructor", Modifiers(Public, true, false), NATIVE(Process::Constructor));
        addNativeMethod("Start", Modifiers(Public, true, false), NATIVE(Process::Start));
    }

    NATIVE_FUNCTION(Constructor) {
        auto name = args[0]->toString();
        handle->addMember(handle, "cmd", args[0]);
        return nullptr;
    }

    NATIVE_FUNCTION(Start) {
        //auto pHandle = (ObjectType*)handle;
        auto pCmd = handle->get(handle, "cmd")->toString();
        return new StringType(Process::execProcess(pCmd.c_str()));
    }

    static std::string execProcess(const char *cmd) {
        std::array<char, 128> buffer{};
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }
};


#endif //S2S_PROCESS_H
