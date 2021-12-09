//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_GC_H
#define S2S_GC_H

#include <stdint.h>

class Runtime;

class GC {
private:
    Runtime *runtime;
    void *memory;
    size_t memorySize;
public:
    GC(Runtime *runtime, size_t reserveSize)
    {

    }
};


#endif //S2S_GC_H
