//
//  executer.h
//  ACL_evaluation
//
//  Created by apple  on 10/8/15.
//  Copyright (c) 2015 apple . All rights reserved.
//

#ifndef __ACL_evaluation__executer__
#define __ACL_evaluation__executer__

#include <stdio.h>
#include <string>
#include "files.h"
#include "util.h"
#include "common.h"

class Executer {
public:
  static void exeCreate(std::string path,
                        std::vector<acl_pair> user_permission);
  static void exeDelete(std::string path);
  static void exeModifyACL(std::string path,
                           std::vector<acl_pair> user_permission);
};

#endif /* defined(__ACL_evaluation__executer__) */
