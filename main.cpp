#include <iostream>

#include <string>
#include <memory>
#include <algorithm>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "samefs_searcher/bayansearcher.h"

namespace bfs = boost::filesystem;
namespace bpo = boost::program_options;


int main (int argc, char ** argv){
    //cmd
    size_t min_size;
    size_t block_size;
    size_t max_depth;
    std::vector<std::string> pathes_for_incl;
    std::vector<std::string> pathes_for_excl;
    std::vector<std::string> file_masks;


    bpo::options_description desc("Bayan options");
    desc.add_options()
        ("help,h"       , "use -i include paths, -e exclude paths, -m regexp mask, -z min_size, -b blok size")
        ("include,i"    , bpo::value(&pathes_for_incl), "directories include for search")
        ("exclude,e"    , bpo::value(&pathes_for_excl), "directories exclude from search")
        ("mask,m"       , bpo::value(&file_masks), "mask for search")
        ("min-size,z"   , bpo::value(&min_size)->default_value(1), "min file size")
        ("blok-size,b"  , bpo::value(&block_size)->default_value(1024), "block size for hash")
        ("max-depth,d"  , bpo::value(&max_depth)->default_value(10), "max depth for search")
        ;

    bpo::variables_map vm;
    bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
    bpo::notify(vm);

    if (vm.count("help") || vm.count("h")) {
        std::cout << desc << "\n";
        exit(0);
    }


    // ~cmd

    std::unique_ptr<BayanSearcher> bfs = std::make_unique<BayanSearcher>();
    for(auto &mask: file_masks)
        bfs->add_mask(mask);

    for(auto &incl: pathes_for_incl)
        bfs->add_incdirs(incl);

    for(auto &excl: pathes_for_excl)
        bfs->add_excldirs(excl);

    bfs->set_max_depth(max_depth);
    bfs->set_blok_size(block_size);
    bfs->set_min_file_size(min_size);


    bfs->process_files();

    return 0;
}
