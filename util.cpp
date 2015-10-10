//
//  util.cpp
//  ACL_evaluation
//
//  Created by apple  on 10/9/15.
//  Copyright (c) 2015 apple . All rights reserved.
//

#include "util.h"

ACLFile* getParent(std::string path, ACLFile* root){
    ACLFile* p = root;
    std::vector<std::string> part_path;
    split(part_path, path, "/", split::no_empties);
    
    for(size_t n = 0; n<part_path.size()-1 && p!=NULL; ++n){
        if(p->file_link.find("/"+part_path[n])!=p->file_link.end())
            p = p->file_link.at("/"+part_path[n]);
        else
            p = NULL;
    }
    return p==root?NULL:p;
}

std::string SplitFilename ( std::string& str){
    std::size_t found = str.find_last_of("/\\");
    return str.substr(found);
}