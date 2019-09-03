#ifndef FILESTRUCTURE_H
#define FILESTRUCTURE_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>

#include "hash/crc32hash.h"

struct FileStructure{

    FileStructure(std::string arg, size_t sz):m_strPath(arg),m_szLastPosition(0),m_FileSize(sz){
//        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    FileStructure(const FileStructure & fs):m_strPath(fs.m_strPath),m_szLastPosition(fs.m_szLastPosition),m_FileSize(fs.m_FileSize){
//        std::cout << __PRETTY_FUNCTION__ << std::endl;
        for(auto str: fs.m_lstHash)m_lstHash.push_back(str);
    }

    std::string m_strPath;
    std::vector<std::string> m_lstHash;
    size_t  m_szLastPosition;
    size_t  m_FileSize;


    std::string get_hash_at(std::ifstream &fstream ,size_t ipos, size_t blokSize){
        if(ipos > m_szLastPosition || ipos >= m_lstHash.size()){
            // process next hash
            process_file(fstream, ipos, blokSize);
            ++m_szLastPosition;
        }
        return m_lstHash.at(ipos);
    }


    void process_file(std::ifstream &fstream ,size_t ipos ,size_t blokSize){
//        std::cout<< m_szLastPosition << " :" << __PRETTY_FUNCTION__ << " " << m_strPath << std::endl;
        auto buffer = std::unique_ptr<char>(new char[blokSize]);
        if (!fstream.is_open()){
//            std::cout <<" open file stream " << m_strPath << std::endl;
            fstream.open(m_strPath, std::ios::in | std::ios::binary | std::ios::ate);
        }
        fstream.seekg(ipos * blokSize, std::ios_base::beg);
        fstream.read(buffer.get(), blokSize);
        auto crc32 = CRC32Hash();
        m_lstHash.push_back(crc32.hash(buffer.get(), blokSize));
    }

    std::string getFullHash(){
        std::string str;
        auto itbeg = m_lstHash.begin();
        auto itend = m_lstHash.end();
        while (itbeg != itend) {
            str.append(*itbeg);
            ++itbeg;
        }
        return str;
    }


};


#endif // FILESTRUCTURE_H
