#ifndef HASHHOLDER_H
#define HASHHOLDER_H

#include <string>
#include <list>

struct HashHolder{

    HashHolder(std::string&& arg):m_strPath(arg),m_bFilledHash(false){}

    std::string m_strPath;
    std::list<std::string> m_lstHash;
    bool m_bFilledHash;


    std::string get_hash(){
        std::string res;
        for(auto str: m_lstHash)
            res+= str;
        return res;
    }
};

#endif // HASHHOLDER_H
