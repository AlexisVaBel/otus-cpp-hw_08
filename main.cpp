#include <iostream>

#include <string>
#include <memory>
#include <algorithm>

#include <boost/filesystem.hpp>
#include "samefs_searcher/bayansearcher.h"

namespace bfs = boost::filesystem;







int main (int argc, char ** argv){
    bfs::path cwd = bfs::current_path();

    std::unique_ptr<BayanSearcher> bfs = std::make_unique<BayanSearcher>();
    bfs->add_incdirs(bfs::current_path().string());
    bfs->add_excldirs("bayan.dir");
    bfs->add_excldirs("3.7.2");
    bfs->set_max_depth(10);

    // extension change to mask
    bfs->add_extension("txt");
    bfs->add_extension("cbp");
    //~ extension change to mask

    bfs->make_file_path_list();
    bfs->list_files_to_process();
    bfs->compare_files();
    return 0;
}
