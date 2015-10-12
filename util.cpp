//
//  util.cpp
//  ACL_evaluation
//
//  Created by apple  on 10/9/15.
//  Copyright (c) 2015 apple . All rights reserved.
//

#include "util.h"

ACLFile *getParent(std::string path, ACLFile *root) {
  ACLFile *p = root;
  std::vector<std::string> part_path;
  split(part_path, path, "/", split::no_empties);

  for (size_t n = 0; n < part_path.size() - 1 && p != NULL; ++n) {
    if (p->file_link.find("/" + part_path[n]) != p->file_link.end())
      p = p->file_link.at("/" + part_path[n]);
    else
      p = NULL;
  }
  return p;
}

std::string getUser(std::string &user_group) {
  std::size_t found = user_group.find(".");
  return user_group.substr(0, found);
}

std::string SplitFilename(std::string &str) {
  std::size_t found = str.find_last_of("/\\");
  return str.substr(found);
}

std::string trim(const std::string &str) {
  std::string whitespace = " \t";
  const auto strBegin = str.find_first_not_of(whitespace);
  if (strBegin == std::string::npos)
    return ""; // no content

  const auto strEnd = str.find_last_not_of(whitespace);
  const auto strRange = strEnd - strBegin + 1;

  return str.substr(strBegin, strRange);
}

void formatPrint(int command_number, std::string op_result, std::string command,
                 std::string err_message) {
  std::cout << command_number << "\t" << op_result << "\t" << command << "\t"
            << err_message << std::endl;
}

void formatPrint(int line_Number, std::string op_result,
                 std::string err_message) {
  std::cout << line_Number << "\t" << op_result << "\t" << err_message
            << std::endl;
}

int translatePermission(std::string per) {
  if (per == "r")
    return 2;
  if (per == "w")
    return 1;
  if (per == "rw")
    return 3;
  if (per == "-")
    return 0;
  return 0;
}