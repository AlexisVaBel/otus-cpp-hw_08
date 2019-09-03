#include "crc32hash.h"

#include <boost/crc.hpp>

CRC32Hash::CRC32Hash()
{

}

std::string CRC32Hash::hash(const char *buff, size_t size)
{
    boost::crc_32_type hashF;
    hashF.process_bytes (buff, size);
    return std::to_string(hashF.checksum());
}
