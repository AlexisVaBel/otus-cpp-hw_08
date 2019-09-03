#ifndef BAYANSEARCHER_H
#define BAYANSEARCHER_H


#include "files/filestructure.h"

#include <string>
#include <list>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>

static const size_t BAYAN_MIN_FILE_SIZE = 1;
static const size_t BAYAN_BLOK_SIZE     = 512;
static const size_t BAYAN_MAX_DEPTH     = 1;


class BayanSearcher
{
public:
    BayanSearcher();

    void process_files  ();

    void add_mask       (std::string &str);
    void add_incdirs    (std::string str);
    void add_excldirs   (std::string str);

    void set_min_file_size  (size_t iArg){m_minFileSize  = iArg;}
    void set_blok_size      (size_t iArg){m_blokSize     = iArg;}
    void set_max_depth      (size_t iArg){m_maxDepth     = iArg;}

    void make_file_path_list();
    void compare_files();
    void print_same_files_paths();


// debug members
    void list_files_to_process();
//~debug members
private:
    size_t  m_minFileSize;
    size_t  m_blokSize;
    size_t  m_maxDepth;

    std::list<std::string> m_lstInc;
    std::list<std::string> m_lstExcl;
    std::list<boost::regex> m_lstMask;


    std::vector<FileStructure> m_lstFStructured;


    std::map <std::string, std::unordered_set<std::string> > m_mapSameFiles; // some name and list of all directories


    void process_file(FileStructure &fstr1, FileStructure &fstr2);

    void process_entry(const boost::filesystem::path& dirpath, size_t iDepth);
};

#endif // BAYANSEARCHER_H
