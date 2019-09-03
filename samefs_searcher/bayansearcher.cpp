#include "bayansearcher.h"
#include "iostream"

#include <fstream>
#include <boost/range/iterator_range.hpp>

namespace bfs = boost::filesystem;

BayanSearcher::BayanSearcher():m_minFileSize(BAYAN_MIN_FILE_SIZE),m_blokSize(BAYAN_BLOK_SIZE), m_maxDepth(BAYAN_MAX_DEPTH)
{
}

void BayanSearcher::process_files()
{
    make_file_path_list();
    compare_files();
    print_same_files_paths();
}

void BayanSearcher::add_mask(std::string& str)
{
    m_lstMask.emplace_back(str);
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
    for(size_t i =0 ; i < m_lstFStructured.size(); ++i){
        for(size_t j = i+1 ; j < m_lstFStructured.size(); ++j){
            process_file(m_lstFStructured.at(i),m_lstFStructured.at(j));
        }
    }
}

void BayanSearcher::print_same_files_paths()
{
    for(auto keyvals: m_mapSameFiles){
        for(auto val: keyvals.second){
            std::cout << val << std::endl;
        }
        std::cout << std::endl;
    }
}

// debug
void BayanSearcher::list_files_to_process()
{
    for(auto str: m_lstFStructured){
        std::cout << str.m_strPath << std::endl;
    }
}
//~debug

void BayanSearcher::process_file(FileStructure &fstr1, FileStructure &fstr2)
{
    if(fstr1.m_FileSize != fstr2.m_FileSize) return;      // nothing to do on no files missmatched size

    auto blokCnt    = fstr1.m_FileSize / m_blokSize + 1;
    std::ifstream fstream1;
    std::ifstream fstream2;
    for(size_t i = 0; i < blokCnt; ++i){
        if(fstr1.get_hash_at(fstream1, i, m_blokSize).compare(fstr2.get_hash_at(fstream2, i, m_blokSize)) != 0 ){
            return;
        }
    }
    //  duplicates
    auto it = m_mapSameFiles.insert(std::pair<std::string, std::unordered_set<std::string>>(fstr2.getFullHash(), std::unordered_set<std::string>())).first;
    it->second.insert(fstr1.m_strPath);
    it->second.insert(fstr2.m_strPath);
    // ~duplicates
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

            size_t fileSize = file_size(entry.path());
            if(fileSize < m_minFileSize) continue;

            if(m_lstMask.empty()){
                FileStructure fls (entry.path().string(),fileSize);
                m_lstFStructured.push_back(fls);

            }else{
                static boost::smatch matchedResult;
                auto   strFileCur = entry.path().filename().string();
                for(const auto &strMask: m_lstMask){
                    if (boost::regex_match(strFileCur, matchedResult, strMask)){
                        FileStructure fls (entry.path().string(),fileSize);
                        m_lstFStructured.push_back(fls);
                    }
                }
            }
        }

    }

}
