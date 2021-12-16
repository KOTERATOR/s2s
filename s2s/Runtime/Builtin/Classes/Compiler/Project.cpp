//
// Created by iashi on 16.12.2021.
//

#include <Runtime/Builtin/Classes/Hash.h>
#include "Project.h"

ObjectType *Project::createInstance(Runtime *r) {
    auto ph = new ProjectHandle(this);
    ph->loadCache();
    return ph;
}

bool Project::checkChange(const std::string &filename, ProjectHandle *handle) {
    auto sha256 = new HashSHA256(handle->hash);

    std::ifstream in;
    in.open(filename);
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        sha256->update(line);
    }

    auto h = sha256->digest();
    std::string newHash = HashSHA256::digestToString(h);
    delete [] h;

    if (handle->filesHashes.find(filename) != handle->filesHashes.end()) {
        std::string oldHash = handle->filesHashes[filename];
        std::cout << newHash << " " << oldHash << std::endl;
        if (newHash != oldHash) {
            handle->filesHashes[filename] = newHash;
            return true;
        }
        return false;
    } else
    {
        handle->filesHashes[filename] = newHash;
        return true;
    }
}

void Project::saveCache(ProjectHandle *ph) {
    ph->saveCache();
}

ArrayType *Project::getArray(const std::string &name, Type *handle) {
    ArrayType *sources = nullptr;
    auto ph = (ProjectHandle*)handle;
    if (handle->hasMember("sources")) {
        auto sourcesHandle = ph->get(handle, "sources", false);
        if (IS_ARRAY(sourcesHandle))
            sources = TO_ARRAY(sourcesHandle);
    }
    else {
        sources = new ArrayType();
        ph->addMember(ph, "sources", sources);
    }
    return sources;
}
