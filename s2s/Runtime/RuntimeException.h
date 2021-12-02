//
// Created by iashi on 27.11.2021.
//

#ifndef S2S_RUNTIMEEXCEPTION_H
#define S2S_RUNTIMEEXCEPTION_H

#include <exception>
#include <string>

class RuntimeException : std::exception {
private:
    std::string str;
public:
    RuntimeException(std::string str) : str(std::move(str)) {}

    const char * what() const noexcept override
    {
        return str.c_str();
    }
};


#endif //S2S_RUNTIMEEXCEPTION_H
