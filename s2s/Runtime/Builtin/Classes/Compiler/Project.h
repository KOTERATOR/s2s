//
// Created by iashi on 16.12.2021.
//

#ifndef S2S_PROJECT_H
#define S2S_PROJECT_H


#include <Runtime/Builtin/Types/ClassType.h>
#include <Runtime/Builtin/Types/IntType.h>
#include <Runtime/Builtin/Types/ArrayType.h>
#include <Runtime/Builtin/Classes/Process.h>

//#include <filesystem>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

class ProjectHandle;

class Project : public ClassType {
private:
    ArrayType *getArray(const std::string &name, Type *handle);
public:
    const char * BUILD_DIR = "S2S_BUILD";
    const char * CACHE_DIR = "S2S_CACHE";

    Project(Block *b) : ClassType(b) {
        ensureBuildDir();
        addNativeMethod("Constructor", Modifiers(Public, true, false), NATIVE(Project::Constructor));
        addNativeMethod("AddSources", Modifiers(Public, true, false), NATIVE(Project::AddSources));
        addNativeMethod("Compile", Modifiers(Public, true, false), NATIVE(Project::Compile));
    }

    ObjectType *createInstance(Runtime *r, Args args, KWArgs kwargs) override;

    NATIVE_FUNCTION(Constructor) {
        std::cout << "Project::ctor" << std::endl;
    }

    NATIVE_FUNCTION(AddSources) {
        ArrayType *sources = getArray("sources", handle);
        for (auto &a : args) {
            std::cout << "Added source: " << a->toString() << std::endl;
            sources->values.emplace_back(a);
        }
        return nullptr;
    }

    NATIVE_FUNCTION(Compile) {
        auto ph = (ProjectHandle*)handle;
        ArrayType *sources = getArray("sources", handle);
        std::vector<std::string> filesToCompile;
        std::vector<std::string> objs;
        for (auto &s : sources->values) {
            if (IS_STRING(s))
            {
                auto filename = TO_STRING(s)->str;
                if (checkChange(filename, ph)) {
                    filesToCompile.emplace_back(filename);
                }
                else if (!fs::exists(fs::path(BUILD_DIR) / fs::path(filename + ".o")))
                {
                    std::cout << "Object " << filename + ".o" << " - not found.";
                    filesToCompile.emplace_back(filename);
                }
                objs.emplace_back((fs::path(BUILD_DIR) / fs::path(filename)).string());
            }
        }

        if (!filesToCompile.empty()) {
            std::cout << "Compiling..." << std::endl;
            for (auto &f : filesToCompile) {
                std::cout << "Compiling " << f << std::endl;
                std::cout << Process::execProcess(("g++ -g -O -c " + f + " -o " + (fs::path(BUILD_DIR) / fs::path(f + ".o")).string()).c_str()) << std::endl;
            }

            std::string objsStr;
            for (auto &o : objs)
                objsStr += o + ".o ";

            std::cout << Process::execProcess(("g++ " + objsStr + " -o program.exe").c_str()) << std::endl;
        }
        else
        {
            std::cout << "Project up-to-date" << std::endl;
        }
        saveCache(ph);
        return nullptr;
    }

    void saveCache(ProjectHandle *ph);

    bool checkChange(const std::string &filename, ProjectHandle *handle);

    void ensureBuildDir() {
        if (!fs::is_directory(BUILD_DIR) || !fs::exists(BUILD_DIR)) {
            fs::create_directory(BUILD_DIR);
        }
    }
};

class ProjectHandle : public ObjectType {
public:
    Hash *hash;
    const char * CACHE_FILE = "S2S_CACHE.txt";
    fs::path buildDir;
    fs::path cacheDir;
    fs::path cacheFile;
    std::map<std::string, std::string> filesHashes;
    ProjectHandle(ClassType *t) : ObjectType(t) {
        auto p = (Project*)type;
        fs::path build_dir(p->BUILD_DIR);
        fs::path cache_dir(p->CACHE_DIR);
        fs::path cache_file(CACHE_FILE);
        buildDir = build_dir;
        cacheDir = build_dir / cache_dir;
        cacheFile = build_dir / cache_dir / cache_file;
        this->hash = new Hash(this);
    }

    void loadCache() {
        if (!fs::is_directory(cacheDir) || !fs::exists(cacheDir))
            fs::create_directory(cacheDir);
        if (fs::is_directory(cacheFile) || !fs::exists(cacheFile)) {
            std::ofstream out;
            out.open(cacheFile.c_str());
            out.close();
        }

        std::ifstream in;
        in.open(cacheFile.c_str());

        bool nowFilename = true;
        std::string filename;
        std::string hash;
        while (!in.eof())
        {
            if (nowFilename) {
                std::getline(in, filename);
                nowFilename = false;
            }
            else {
                std::getline(in, hash);
                nowFilename = true;
                std::cout << "Added hash for " << filename << ": " << hash << std::endl;
                filesHashes[filename] = hash;
            }
        }
    }

    void saveCache() {
        std::ofstream out;
        out.open(cacheFile.c_str());
        for (const auto &f : filesHashes) {
            out << f.first << std::endl << f.second << std::endl;
        }
        out.close();
    }
};



#endif //S2S_PROJECT_H
