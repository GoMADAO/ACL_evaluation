//
//  executer.cpp
//  ACL_evaluation
//
//  Created by apple  on 10/8/15.
//  Copyright (c) 2015 apple . All rights reserved.
//

#include "executer.h"
void Executer::exeCreate(std::string path,
                         std::vector<acl_pair> user_permission) {
  ACLFile *parent = getParent(path, fs->root);
  std::string file_name = SplitFilename(path);

  //  inherits the ACL of its parent
  if (user_permission.size() == 0)
    user_permission = parent->file_permission;

  parent->file_link.insert(
      std::make_pair(file_name, new ACLFile(file_name, user_permission)));
}

void Executer::exeDelete(std::string path) {
  ACLFile *parent = getParent(path, fs->root);
  std::string file_name = SplitFilename(path);
  ACLFile *file = parent->file_link.at(file_name);
  parent->file_link.erase(file_name);
  delete file;
}

void Executer::exeModifyACL(std::string path,
                            std::vector<acl_pair> user_permission) {
  std::string file_name = SplitFilename(path);
  ACLFile *file = getParent(path, fs->root);
  file = file->file_link.at(file_name);
  file->file_permission.clear();
  size_t n = user_permission.size();
  if (user_permission[n - 1].first != "*.*")
    user_permission.push_back(acl_pair("*.*", 0));
  file->file_permission.insert(file->file_permission.end(),
                               user_permission.begin(), user_permission.end());
}
