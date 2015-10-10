//
//  files.cpp
//  ACL_evaluation
//
//  Created by apple  on 10/2/15.
//  Copyright (c) 2015 apple . All rights reserved.
//

#include "files.h"
#include <iostream>

ACLFile::ACLFile(std::string name) { file_name = name; }

ACLFile::ACLFile(std::string name,
                 std::vector<std::string> default_user_permission) {
  file_name = name;
  for (std::vector<std::string>::iterator it = default_user_permission.begin();
       it != default_user_permission.end(); ++it) {
    file_permission.push_back(acl_pair(*it, 3));
  }
  file_permission.push_back(acl_pair("*.*", 2));
}

ACLFile::~ACLFile() {
  // recursively free all the memory of ACLFiles
  for (std::map<std::string, ACLFile *>::iterator it = file_link.begin();
       it != file_link.end(); ++it)
    delete it->second;
}
