//
// Created by iashi on 21.12.2021.
//

#ifndef S2S_FILE_H
#define S2S_FILE_H

#include <Runtime/Builtin/Types/ClassType.h>
#include <Runtime/Builtin/Types/ObjectType.h>
#include <Runtime/Builtin/Types/BoolType.h>
#include <Runtime/Builtin/Types/StringType.h>

#include <fstream>

class FileHandle : public ObjectType {
public:
    std::fstream file;
    FileHandle(ClassType *t) : ObjectType(t)
    {

    }

    bool Open(const std::string &path, const std::string &mode) {
        auto m = std::ios::in;
        if (mode == "r")
            m = std::ios::in;
        else if (mode == "w")
            m = std::ios::out;
        else if (mode == "a")
            m = std::ios::app;
        file.open(path, m);
        return file.is_open();
    }

    void Write(const std::string &contents) {
        file << contents;
    }

    std::string ReadLine() {
        std::string str;
        std::getline(file, str);
        return str;
    }

    void Close() {
        file.close();
    }
};

class File : public ClassType {
public:
    File(Block *b) : ClassType(b)
    {
        addNativeMethod("Open", Modifiers(Public, true, false), NATIVE(File::Open));
        addNativeMethod("Write", Modifiers(Public, true, false), NATIVE(File::Write));
        addNativeMethod("ReadLine", Modifiers(Public, true, false), NATIVE(File::ReadLine));
        addNativeMethod("Close", Modifiers(Public, true, false), NATIVE(File::Close));
    }

    ObjectType * createInstance(Runtime *r, Args args, KWArgs kwargs) {
        return new FileHandle(this);
    }

    NATIVE_FUNCTION(Open) {
        return new BoolType(((FileHandle*)handle)->Open(TO_NATIVE_STRING(args[0]), TO_NATIVE_STRING(args[1])));
    }

    NATIVE_FUNCTION(Write) {
        ((FileHandle*)handle)->Write(TO_NATIVE_STRING(args[0]));
        return handle;
    }

    NATIVE_FUNCTION(ReadLine) {
        return new StringType(((FileHandle*)handle)->ReadLine());
    }

    NATIVE_FUNCTION(Close) {
        ((FileHandle*)handle)->Close();
        return handle;
    }
};


#endif //S2S_FILE_H
