//
//  files.h
//  ACL_evaluation
//
//  Created by apple  on 10/2/15.
//  Copyright (c) 2015 apple . All rights reserved.
//

#ifndef __ACL_evaluation__files__
#define __ACL_evaluation__files__

#include <stdio.h>
#include <map>
#include <string>
#include <utility>
#include <vector>
/*
  the permission uses int = {0, 1, 2, 3}, where each number coresponding to rw
 permission
  eg. 0 -> - , 1->w, 2->r, 3->rw

 */
typedef std::pair<std::string, int> acl_pair;
inline bool operator==(const acl_pair &lhs, const acl_pair &rhs) {
  if (lhs.first == rhs.first)
    return true;
  return false;
}

class ACLFile {
public:
  /*
   file_link: a link to child directory
   file_name: name of this file
   file_permission: an array which stores pairs of permission in the format
   <user_group, permission>
   */
  std::string file_name;
  std::map<std::string, ACLFile *> file_link;
  std::vector<acl_pair> file_permission;
  ACLFile(std::string name);
  //  setting default_user_permission. called in user definition section
  ACLFile(std::string name, std::vector<std::string> default_user_permission);
  //  setting user_specified_permission. called in operation portion
  ACLFile(std::string name, std::vector<acl_pair> specified_user_permission);

  ~ACLFile();
};

#endif /* defined(__ACL_evaluation__files__) */
