#ifndef CRC32HASH_H
#define CRC32HASH_H

#include "hash/ihash.h"
class CRC32Hash:public IHash
{
public:
    CRC32Hash();

    // IHash interface
public:
    std::string hash(const char *buff, size_t size);
};

#endif // CRC32HASH_H
