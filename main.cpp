//
//  main.cpp
//  ACL_evaluation
//
//  Created by apple  on 10/1/15.
//  Copyright (c) 2015 apple . All rights reserved.
//

#include <iostream>
#include <string>
#include <map>
#include <set>
#include "checker.h"
#include "common.h"
#include "filesystem.h"
#include "files.h"
#include "util.h"
using namespace std;

FileSystem *fs;

void testMap() {
  ACLFile *a;
  a = new ACLFile("hello");
  cout << a->file_name << endl;
  cout << a->file_link.size() << endl;
  acl_pair x("hhh", 1);
  a->file_permission.push_back(x);
  ACLFile temp = *a;
  cout << temp.file_permission.at(0).first << endl;
  cout << "Hello, World!\n";
  delete a;
  cout << fs->root->file_link.at("/home")->file_name << endl;
  cout << fs->root->file_link.at("/tmp")->file_name << endl;
}

void testSplit() {
  std::string s = "smb.faculty /home/smb";
  std::vector<std::string> fields;
  split(fields, s, " ", split::no_empties);

  for (size_t n = 0; n < fields.size(); ++n) {
    std::cout << fields[n] << std::endl;
  }
}

void testReadPermssion() {
  Checker::readFile(1, "/home/sal", "smb.columbia");
  // Checker::readFile(2,"/home", "smb.columbia");
}

void testWritePermssion() {
  Checker::writeFile(3, "/home/sal", "sal.faculty");
  Checker::writeFile(4, "/home/sal", "smb.columbia");
  vector<acl_pair> user_permission;
  //  user_permission.push_back(acl_pair("*.aaaaa",1));
  //  user_permission.push_back(acl_pair("*.*",0));
  // Checker::modifyACL(5,"/home/sal", "sal.faculty", user_permission);
  Checker::createFile(3, "/home/sal/a", "sal.faculty", user_permission);
  fs->showFilePermission();

  Checker::deleteFile(6, "/home/sal/a", "sal.faculty");
}

bool isValidPath(std::string path) {
  ACLFile *parent = getParent(path, fs->root);
  std::string file_name = SplitFilename(path);
  if (parent->file_link.find(file_name) != parent->file_link.end()) {
    if (parent->file_link.at(file_name)->file_link.size() != 0)
      return false;
  }
  return true;
}

void builfFSwithBufferedData(vector<string> *user_group_list, string file_name,
                             int input_no) {
  if (!user_group_list->empty()) {
    // build file system
    if (isValidPath(file_name)) {
      fs->addUserGroup(user_group_list);
      fs->addACLFile(user_group_list, file_name);
    } else {
      formatPrint(input_no - (int)user_group_list->size(), "X", INVALIDPATH);
    }
    user_group_list->clear();
  }
}

int main(int argc, const char *argv[]) {

  // environment
  fs = new FileSystem;

  // read a line into buff
  string buff;
  string next_line;

  vector<string> tokens;
  vector<string> user_group_list;
  string file_name;
  string temp_user;

  int input_no = 0;
  /* --- user definition section --- */
  while (getline(cin, buff) && buff != ".") {

    ++input_no;

    //  split line into tokens and checke the size
    split(tokens, buff, " ", split::no_empties);

    //  invalid user_group definition
    if (!Checker::isValidUserGroup(tokens[0])) {
      formatPrint(input_no, "X", INVALIDUSERGROUP);
      continue;
    }

    /*---Only User.Group---*/
    if (tokens.size() == 1) {
      if (temp_user.size() == 0 || temp_user != getUser(tokens[0])) {
        formatPrint(input_no, "X", USERNOFILE);
        temp_user.clear();
        //  may have valid previous command
        builfFSwithBufferedData(&user_group_list, file_name, input_no);

      } else {
        //  not finish file permission description
        //  e.g. smb.columbia
        //       smb.faculty
        //  ignore duplicate user_group
        if (find(user_group_list.begin(), user_group_list.end(), tokens[0]) ==
            user_group_list.end())
          user_group_list.push_back(tokens[0]);
      }
      continue;
    }

    /*---Complete Command---*/
    builfFSwithBufferedData(&user_group_list, file_name, input_no);

    //  invalid file_name
    if (!Checker::isValidFileName(tokens[1])) {
      formatPrint(input_no, "X", INVALIDFILENAME);
      continue;
    }
    user_group_list.push_back(tokens[0]);
    file_name = tokens[1];
    temp_user = getUser(tokens[0]);
  }

  // rest of user definition
  builfFSwithBufferedData(&user_group_list, file_name, input_no);

  /* --- command section --- */
  int com_no = 0;
  vector<string> permission_pair;
  vector<acl_pair> specified_permission;
  while (getline(cin, buff) && buff != "==") {
    ++com_no;

    split(tokens, buff, " ", split::no_empties);

    if (tokens.size() != 3) {
      formatPrint(com_no, "X", buff, INVALIDINPUT);
      continue;
    }

    //  invalid user_group definition
    if (!Checker::isValidUserGroup(tokens[1])) {
      formatPrint(com_no, "X", buff, INVALIDUSERGROUP);
      continue;
    }

    //  invalid file_name
    if (!Checker::isValidFileName(tokens[2])) {
      formatPrint(input_no, "X", buff, INVALIDFILENAME);
      continue;
    }

    if (tokens[0] == "READ")
      Checker::readFile(com_no, tokens[2], tokens[1]);
    else if (tokens[0] == "WRITE")
      Checker::writeFile(com_no, tokens[2], tokens[1]);
    else if (tokens[0] == "DELETE")
      Checker::deleteFile(com_no, tokens[2], tokens[1]);
    else if (tokens[0] == "CREATE" || tokens[0] == "ACL") {

      while (getline(cin, buff) && buff != ".") {
        split(permission_pair, buff, " ", split::no_empties);
        //  invalid user_group definition
        if (!Checker::isValidUserGroup(permission_pair[0])) {
          formatPrint(com_no, "X", buff, INVALIDUSERGROUP);
          continue;
        }
        specified_permission.push_back(acl_pair(
            permission_pair[0], translatePermission(permission_pair[1])));
      }
      if (tokens[0] == "CREATE")
        Checker::createFile(com_no, tokens[2], tokens[1], specified_permission);
      else
        Checker::modifyACL(com_no, tokens[2], tokens[1], specified_permission);
      specified_permission.clear();
    } else {
      formatPrint(com_no, "X", buff, NOTFOUNDOPERATION);
    }
  }

  //  free memory
  delete fs;

  return 0;
}
