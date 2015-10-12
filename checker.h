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
#include "executer.h"
#include "common.h"

class Checker {
  /*
      Checker does the checking
      If succeed, it then calls functions in executer for WRITE, DELETE AND ACL
     operation
   */

public:
  //---PERMISSION CHECKING---
  static void readFile(int command_no, std::string path,
                       std::string user_group);
  static void writeFile(int command_no, std::string path,
                        std::string user_group);

  // check parent write permission, for CREATE, DELETE permission
  static void createFile(int command_no, std::string path,
                         std::string user_group,
                         std::vector<acl_pair> user_permission);
  static void deleteFile(int command_no, std::string path,
                         std::string user_group);
  static void modifyACL(int command_no, std::string path,
                        std::string user_group,
                        std::vector<acl_pair> user_permission);

  //---USER DEFINITION CHECKING---
  //  check input is a line separated by a single space
  static bool isValidUserDefInput(std::string input);

  //  check the user_group is valid
  //  must reject cases 1. doesnt contains "." 2. contains multi "." 3. low_case
  //  letter
  static bool isValidUserGroup(std::string user_group);

  static bool isValidFileName(std::string path);
};

#endif /* defined(__ACL_evaluation__checker__) */
