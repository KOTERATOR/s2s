//
// Created by iashi on 04.12.2021.
//

#ifndef S2S_DICTIONARYTYPE_H
#define S2S_DICTIONARYTYPE_H

#include "Type.h"

class DictionaryType : public Type {
protected:
    std::map<Type*, Type*> dict;
public:
    DictionaryType() : Type(TypeEnum::Dictionary) {

    }
};


#endif //S2S_DICTIONARYTYPE_H
