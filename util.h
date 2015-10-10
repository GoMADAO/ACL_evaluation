//
//  util.h
//  ACL_evaluation
//
//  Created by apple  on 10/9/15.
//  Copyright (c) 2015 apple . All rights reserved.
//

#ifndef __ACL_evaluation__util__
#define __ACL_evaluation__util__

#include <stdio.h>
#include <cstddef>
#include <string>
#include <vector>
#include "files.h"
#include "filesystem.h"

struct split
{
    enum empties_t { empties_ok, no_empties };
};

template <typename Container>
Container& split(
                 Container&                            result,
                 const typename Container::value_type& s,
                 const typename Container::value_type& delimiters,
                 split::empties_t                      empties = split::empties_ok )
{
    result.clear();
    size_t current;
    size_t next = -1;
    do
    {
        if (empties == split::no_empties)
        {
            next = s.find_first_not_of( delimiters, next + 1 );
            if (next == Container::value_type::npos) break;
            next -= 1;
        }
        current = next + 1;
        next = s.find_first_of( delimiters, current );
        result.push_back( s.substr( current, next - current ) );
    }
    while (next != Container::value_type::npos);
    return result;
}


std::string SplitFilename ( std::string& str);



//  trace ACL and return parent directory
ACLFile* getParent(std::string path, ACLFile* root);


void formatPrint(std::string path, std::string user_group);




#endif /* defined(__ACL_evaluation__util__) */
