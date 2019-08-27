#include "bayansearcher.h"
#include "iostream"

#include <fstream>

#include <boost/crc.hpp>
#include <boost/range/iterator_range.hpp>

namespace bfs = boost::filesystem;

BayanSearcher::BayanSearcher():m_minFileSize(BAYAN_MIN_FILE_SIZE),m_blokSize(BAYAN_BLOK_SIZE), m_maxDepth(BAYAN_MAX_DEPTH)
{
}

void BayanSearcher::add_extension(std::string str)
{
    m_lstExt.push_back(str);
}

void BayanSearcher::add_incdirs(std::string str)
{
    m_lstInc.push_back(str);
}

void BayanSearcher::add_excldirs(std::string str)
{
    m_lstExcl.push_back(str);
}

void BayanSearcher::make_file_path_list()
{
    for(auto itm: m_lstInc)
        process_entry(itm,0);
}

void BayanSearcher::compare_files()
{
    auto itFst = m_lstFilePaths.begin();
    auto itEnd = m_lstFilePaths.end();

    while(itFst != itEnd){
        auto itSnd = itFst;
        while(itSnd != itEnd){
            if(itSnd != itFst)
                process_file(*itFst,*itSnd);
            ++itSnd;
        }
        ++itFst;
    }
}

// debug
void BayanSearcher::list_files_to_process()
{
    for(auto str: m_lstFilePaths){
        std::cout << str << std::endl;
    }
}
//~debug

void BayanSearcher::process_file(std::string str1, std::string str2)
{
    bfs::path path1(str1);
    bfs::path path2(str2);

    auto cmnFileSize = file_size(path1);             // if file same size, no matter what part to get
    if(cmnFileSize != file_size(path2)) return;      // nothing to do on no files missmatched size




//    result.process_bytes(data, size);
//    std::to_string(result.checksum());
    HashHolder hash1 = m_mapHashFiles.at(str1);
    HashHolder hash2 = m_mapHashFiles.at(str2);
    if(hash1.m_bFilledHash && hash2.m_bFilledHash){
        if ( hash1.get_hash().compare(hash2.get_hash())== 0 ){
            std::cout << " same hash found " << std::endl;
            return;
        }
    }


    auto blokCnt    = cmnFileSize / m_blokSize + 1;

    std::ifstream fStream1;
    std::ifstream fStream2;
    boost::crc_32_type result;

    for(size_t i = 0; i < blokCnt; ++i){

    }


}

void BayanSearcher::process_entry(const boost::filesystem::path &dirpath, size_t iDepth)
{
    if( !is_directory(dirpath) ){
        return;
    }

    auto strCmp = std::string(dirpath.string());

    for( auto strExcl: m_lstExcl){
        if(strCmp.find(strExcl) != std::string::npos){
            return; // something excluding
        }
    }

    for(auto &entry : boost::make_iterator_range(bfs::directory_iterator(dirpath),{})){
        if( iDepth < m_maxDepth) process_entry(entry.path(),iDepth + 1);

        if(is_regular_file(entry.path())){
            auto strExtCur = entry.path().extension().string();
            if(m_lstExt.empty()){
                if(file_size(entry.path()) >= m_minFileSize)m_lstFilePaths.push_back(entry.path().string());
            }else
            for( auto strExt: m_lstExt){
                if(strExtCur.find(strExt) != std::string::npos){
                    if(file_size(entry.path()) >= m_minFileSize)m_lstFilePaths.push_back(entry.path().string());
                }
            }
        }

    }

}
