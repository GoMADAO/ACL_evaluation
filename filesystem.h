//
//  filesystem.h
//  ACL_evaluation
//
//  Created by apple  on 10/8/15.
//  Copyright (c) 2015 apple . All rights reserved.
//

#ifndef __ACL_evaluation__filesystem__
#define __ACL_evaluation__filesystem__

#include <stdio.h>
#include <map>
#include <set>
#include <string>
#include "files.h"
#include "util.h"


#include <iostream>

/*
  users: all valid users
  groups: all valid groups
  root: file system root
 */
class FileSystem {
public:
  std::map<std::string, std::set<std::string>> users;
  std::map<std::string, std::set<std::string>> groups;
  ACLFile *root;
  FileSystem();
  ~FileSystem();
  /*    
      user definition section:
   */
  //  initial filesystem users and groups
  void addUserGroup(std::vector<std::string>* user_group);
  //  initial filesystem file associated with its permission
  void addACLFile(std::vector<std::string>* user_group, std::string file_path);
  
  
  /*
      print functions used to debug
   */
  void showUserGroup();
  void showFilePermission();
  
  
};

#endif /* defined(__ACL_evaluation__filesystem__) */
