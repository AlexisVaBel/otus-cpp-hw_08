#ifndef IHASH_H
#define IHASH_H

#include <string>

struct IHash{
    virtual std::string hash(const char* buff, size_t size) = 0;
    virtual ~IHash(){
    }
};

#endif // IHASH_H
