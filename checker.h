//
//  checker.h
//  ACL_evaluation
//
//  Created by apple  on 10/2/15.
//  Copyright (c) 2015 apple . All rights reserved.
//

#ifndef __ACL_evaluation__checker__
#define __ACL_evaluation__checker__

#include <stdio.h>
#include <string>
#include <set>
#include "files.h"
#include "util.h"
#include "common.h"

class Checker {
public:
  static void read(std::string path, std::string user_group);
  static void write(std::string path, std::string user_group);
  //  create and delete
  static void creadel(std::string path, std::string user_group);
  static void acl(std::string path, std::string user_group);
  

  
};

#endif /* defined(__ACL_evaluation__checker__) */
