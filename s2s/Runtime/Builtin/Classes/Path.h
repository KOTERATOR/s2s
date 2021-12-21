//
// Created by iashi on 21.12.2021.
//

#ifndef S2S_PATH_H
#define S2S_PATH_H

#include <Runtime/Builtin/Types/ClassType.h>
#include <Runtime/Builtin/Types/StringType.h>
#include <Runtime/Builtin/Types/BoolType.h>

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

class Path : public ClassType {
public:
    Path(Block *block) : ClassType(block) {
        addNativeMethod("Exists", Modifiers(Public, true, true), NATIVE(Path::Exists));
        addNativeMethod("CreateFolder", Modifiers(Public, true, true), NATIVE(Path::CreateFolder));
        addNativeMethod("Combine", Modifiers(Public, true, true), NATIVE(Path::Combine));
    }

    NATIVE_FUNCTION(Exists) {
        return new BoolType(Exists(TO_NATIVE_STRING(args[0])));
    }

    bool Exists(const std::string &path) {
        return fs::exists(path);
    }

    NATIVE_FUNCTION(CreateFolder) {
        return new BoolType(CreateFolder(TO_NATIVE_STRING(args[0])));
    }

    bool CreateFolder(const std::string &path) {
        return fs::create_directories(path);
    }

    NATIVE_FUNCTION(Combine) {
        return new StringType(Combine(TO_NATIVE_STRING(args[0]), TO_NATIVE_STRING(args[1])).string());
    }

    fs::path Combine(const fs::path& p1, const fs::path& p2) {
        return p1 / p2;
    }

};


#endif //S2S_PATH_H
