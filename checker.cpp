//
//  checker.cpp
//  ACL_evaluation
//
//  Created by apple  on 10/2/15.
//  Copyright (c) 2015 apple . All rights reserved.
//

#include "checker.h"

/*
 the file already has files in it
 a.b /home/f/b
 c.d /home/f
 thus, f has entry and could not be modified by c.d /home/f command
 */

/*
 * ----------------User-Def Checking Public Interface----------------
 */

bool Checker::isValidUserDefInput(std::string input) {
  std::vector<std::string> temp;
  split(temp, trim(input), " ");
  if (temp.size() != 3)
    return false;
  return true;
}

bool Checker::isValidUserGroup(std::string user_group) {
  if (user_group.at(0) == '*' || user_group.at(user_group.size() - 1) == '*')
    return true;

  int count = 0;
  for (std::string::iterator it = user_group.begin(); it != user_group.end();
       ++it) {
    if (*it == 46) {
      ++count;
      continue;
    }
    if (*it > 122 || *it < 97)
      return false;
  }
  return count == 1;
}

bool Checker::isValidFileName(std::string path) {
  if (path.size() > 256)
    return false;
  if (path.at(0) != 47)
    return false;
  std::vector<std::string> temp;
  split(temp, path, "/", split::no_empties);
  for (size_t n = 0; n < temp.size(); ++n) {
    if (temp[n].size() > 16)
      return false;
    for (std::string::iterator it = temp[n].begin(); it != temp[n].end();
         ++it) {
      if (*it == 46)
        continue;
      else if ((*it > 64 && *it < 91) || (*it > 96 && *it < 123))
        continue;
      else
        return false;
    }
  }
  return true;
}

/*
 * ----------------END User-Def Checking Public----------------
 */

/*
 * ----------------Private Check Helper Functions----------------
 */

// first check exists Previous Path component by calling getParent
bool hasEntry(ACLFile *current) {
  if (current->file_link.size() == 0)
    return false;
  return true;
}

// permission 2 -> r, 3 -> rw, return true
bool isReadable(int r) { return r >= 2 ? true : false; }

// permission 1 -> w, 3 -> rw, return true
bool isWritable(int r) { return (r & 1) == 1 ? true : false; }

// check a specific file read permission
bool canRead(std::string user, std::string group, ACLFile *f) {
  //  permission associated with current file_name
  std::vector<acl_pair> permissions = f->file_permission;
  std::string user_group = user + "." + group;
  std::set<std::string> groups_contains_user = fs->users.at(user);

  //  temp user_group, user, group
  std::string temp;
  std::string temp_user, temp_group;

  for (size_t n = 0; n < permissions.size(); ++n) {
    temp = permissions[n].first;

    if (temp == user_group) {
      return isReadable(permissions[n].second);

    } else {
      std::size_t found = temp.find(".");
      temp_user = temp.substr(0, found);
      temp_group = temp.substr(found + 1);

      if (temp_group == "*" && temp_user == "*") {
        return isReadable(permissions[n].second);

      } else if (temp_user == "*" &&
                 groups_contains_user.find(temp_group) !=
                     groups_contains_user.end()) {
        //  check whether current user's group contains temp_group
        return isReadable(permissions[n].second);

      } else if (temp_group == "*" && user == temp_user) {
        return isReadable(permissions[n].second);
      }
    }
  }
  return false;
}

// check a specific file read permission
bool canWrite(std::string user, std::string group, ACLFile *f) {
  //  permission associated with current file_name
  std::vector<acl_pair> permissions = f->file_permission;
  std::string user_group = user + "." + group;
  std::set<std::string> groups_contains_user = fs->users.at(user);

  //  temp user_group, user, group
  std::string temp;
  std::string temp_user, temp_group;

  for (size_t n = 0; n < permissions.size(); ++n) {
    temp = permissions[n].first;

    if (temp == user_group) {
      return isWritable(permissions[n].second);

    } else {
      std::size_t found = temp.find(".");
      temp_user = temp.substr(0, found);
      temp_group = temp.substr(found + 1);

      if (temp_group == "*" && temp_user == "*") {
        return isWritable(permissions[n].second);

      } else if (temp_user == "*" &&
                 groups_contains_user.find(temp_group) !=
                     groups_contains_user.end()) {
        //  check whether current user's group contains temp_group
        return isWritable(permissions[n].second);

      } else if (temp_group == "*" && user == temp_user) {
        return isWritable(permissions[n].second);
      }
    }
  }
  return false;
}

//  check the path of files read permission
bool canReadPath(std::string path, std::string user, std::string group) {
  std::vector<std::string> parts;
  split(parts, path, "/", split::no_empties);

  ACLFile *p = fs->root;

  for (size_t n = 0; n < parts.size() - 1; ++n) {
    p = p->file_link.at("/" + parts[n]);
    if (!canRead(user, group, p))
      return false;
  }
  return true;
}

bool userExists(std::string &user) {
  return fs->users.find(user) != fs->users.end();
}

bool groupExists(std::string &group) {
  return fs->groups.find(group) != fs->groups.end();
}

/*
 * ----------------END Private Check Helper Functions----------------
 */

/*
 * ----------------Permission Checking Public Interface----------------
 */

//  public interface of read with format output
void Checker::readFile(int command_no, std::string path,
                       std::string user_group) {

  std::vector<std::string> u_g;
  split(u_g, user_group, ".");

  //  command content
  std::string comm = "READ " + user_group + " " + path;

  if (!userExists(u_g[0])) {
    formatPrint(command_no, "X", comm, USERNOTEXISTS);
    return;
  }

  if (!groupExists(u_g[1])) {
    formatPrint(command_no, "X", comm, GROUPNOTEXISTS);
    return;
  }

  ACLFile *parent = getParent(path, fs->root);

  if (parent == NULL) {
    // format print not find path to the file
    formatPrint(command_no, "X", comm, FAILEDFINDPATH);
    return;
  }

  if (!canReadPath(path, u_g[0], u_g[1])) {
    // format print can not read the path
    formatPrint(command_no, "N", comm, FAILEDREADPATH);
    return;
  }

  std::string file_name = SplitFilename(path);
  if (parent->file_link.find(file_name) == parent->file_link.end()) {
    //  format print not found file
    formatPrint(command_no, "X", comm, FILENOTEXIST);
    return;
  }

  if (!canRead(u_g[0], u_g[1], parent->file_link.at(file_name))) {
    //  format print not read permission on current file
    formatPrint(command_no, "N", comm, NOREADPERMISSION);
    return;
  }

  // succeed
  formatPrint(command_no, "Y", comm, SUCCEED);

  return;
}

void Checker::writeFile(int command_no, std::string path,
                        std::string user_group) {

  std::vector<std::string> u_g;
  split(u_g, user_group, ".");

  //  command content
  std::string comm = "WRITE " + user_group + " " + path;

  if (!userExists(u_g[0])) {
    formatPrint(command_no, "X", comm, USERNOTEXISTS);
    return;
  }

  if (!groupExists(u_g[1])) {
    formatPrint(command_no, "X", comm, GROUPNOTEXISTS);
    return;
  }

  ACLFile *parent = getParent(path, fs->root);

  if (parent == NULL) {
    // format print not find path to the file
    formatPrint(command_no, "X", comm, FAILEDFINDPATH);
    return;
  }

  if (!canReadPath(path, u_g[0], u_g[1])) {
    // format print can not read the path
    formatPrint(command_no, "N", comm, FAILEDREADPATH);
    return;
  }

  std::string file_name = SplitFilename(path);
  if (parent->file_link.find(file_name) == parent->file_link.end()) {
    //  format print not found file
    formatPrint(command_no, "X", comm, FILENOTEXIST);
    return;
  }

  if (!canWrite(u_g[0], u_g[1], parent->file_link.at(file_name))) {
    //  format print not write permission on current file
    formatPrint(command_no, "N", comm, NOWRITEPERMISSION);
    return;
  }

  // succeed
  formatPrint(command_no, "Y", comm, SUCCEED);
}

bool canCreate(std::string path, std::string user_group, int command_no) {

  std::vector<std::string> u_g;
  split(u_g, user_group, ".");

  //  command content
  std::string comm = "CREATE " + user_group + " " + path;

  ACLFile *parent = getParent(path, fs->root);

  if (!userExists(u_g[0])) {
    formatPrint(command_no, "X", comm, USERNOTEXISTS);
    return false;
  }

  if (!groupExists(u_g[1])) {
    formatPrint(command_no, "X", comm, GROUPNOTEXISTS);
    return false;
  }

  if (parent == NULL) {
    // format print not find path to the file
    formatPrint(command_no, "X", comm, FAILEDFINDPATH);
    return false;
  }

  if (!canReadPath(path, u_g[0], u_g[1])) {
    // format print can not read the path
    formatPrint(command_no, "N", comm, FAILEDREADPATH);
    return false;
  }

  std::string file_name = SplitFilename(path);
  if (parent->file_link.find(file_name) != parent->file_link.end()) {
    //  format print not found file
    formatPrint(command_no, "X", comm, FILEALREADYEXIST);
    return false;
  }

  if (!canWrite(u_g[0], u_g[1], parent)) {
    //  format print not write permission on current file
    formatPrint(command_no, "N", comm, NOWRITEPERMISSIONPARENT);
    return false;
  }

  // succeed
  formatPrint(command_no, "Y", comm, SUCCEED);
  return true;
}

bool canDelete(std::string path, std::string user_group, int command_no) {

  std::vector<std::string> u_g;
  split(u_g, user_group, ".");

  //  command content
  std::string comm = "DELETE " + user_group + " " + path;

  if (!userExists(u_g[0])) {
    formatPrint(command_no, "X", comm, USERNOTEXISTS);
    return false;
  }

  if (!groupExists(u_g[1])) {
    formatPrint(command_no, "X", comm, GROUPNOTEXISTS);
    return false;
  }

  ACLFile *parent = getParent(path, fs->root);

  if (parent == NULL) {
    // format print not find path to the file
    formatPrint(command_no, "X", comm, FAILEDFINDPATH);
    return false;
  }

  if (!canReadPath(path, u_g[0], u_g[1])) {
    // format print can not read the path
    formatPrint(command_no, "N", comm, FAILEDREADPATH);
    return false;
  }

  std::string file_name = SplitFilename(path);
  if (parent->file_link.find(file_name) == parent->file_link.end()) {
    //  format print not found file
    formatPrint(command_no, "X", comm, FILENOTEXIST);
    return false;
  }

  if (!canWrite(u_g[0], u_g[1], parent)) {
    //  format print not write permission on current file
    formatPrint(command_no, "N", comm, NOWRITEPERMISSIONPARENT);
    return false;
  }

  // succeed
  formatPrint(command_no, "Y", comm, SUCCEED);
  return true;
}

void addNewUserOrGroup(std::vector<acl_pair> user_permission) {
  //  meet new user or group
  std::vector<std::string> vec;
  std::string temp;
  size_t found;
  for (std::vector<acl_pair>::iterator it = user_permission.begin();
       it != user_permission.end(); ++it) {
    temp = it->first;
    found = temp.find(".");
    if (fs->users.find(temp.substr(0, found)) == fs->users.end() ||
        fs->groups.find(temp.substr(found + 1)) == fs->groups.end()) {
      vec.push_back(temp);
    }
  }
  fs->addUserGroup(&vec);
}

void Checker::createFile(int command_no, std::string path,
                         std::string user_group,
                         std::vector<acl_pair> user_permission) {
  if (!canCreate(path, user_group, command_no))
    return;

  std::vector<std::string> u_g;
  split(u_g, user_group, ".");

  //  add new user or group new user or group(not defined by user-def section)
  addNewUserOrGroup(user_permission);

  Executer::exeCreate(path, user_permission);
}

void Checker::deleteFile(int command_no, std::string path,
                         std::string user_group) {
  if (!canDelete(path, user_group, command_no))
    return;

  Executer::exeDelete(path);
}

void Checker::modifyACL(int command_no, std::string path,
                        std::string user_group,
                        std::vector<acl_pair> user_permission) {

  std::string comm = "ACL " + user_group + " " + path;

  std::vector<std::string> u_g;
  split(u_g, user_group, ".");

  if (!userExists(u_g[0])) {
    formatPrint(command_no, "X", comm, USERNOTEXISTS);
    return;
  }

  if (!groupExists(u_g[1])) {
    formatPrint(command_no, "X", comm, GROUPNOTEXISTS);
    return;
  }

  if (user_permission.size() == 0) {
    // format print empty ACL file not allowed
    formatPrint(command_no, "X", comm, EMPTYACL);
    return;
  }

  ACLFile *parent = getParent(path, fs->root);

  if (parent == NULL) {
    // format print not find path to the file
    formatPrint(command_no, "X", comm, FAILEDFINDPATH);
    return;
  }

  if (!canReadPath(path, u_g[0], u_g[1])) {
    // format print can not read the path
    formatPrint(command_no, "N", comm, FAILEDREADPATH);
    return;
  }

  std::string file_name = SplitFilename(path);
  if (parent->file_link.find(file_name) == parent->file_link.end()) {
    //  format print not found file
    formatPrint(command_no, "X", comm, FILENOTEXIST);
    return;
  }

  if (!canWrite(u_g[0], u_g[1], parent->file_link.at(file_name))) {
    //  format print not write permission on current file
    formatPrint(command_no, "N", comm, NOWRITEPERMISSION);
    return;
  }

  //  add new user or group when needed
  addNewUserOrGroup(user_permission);

  Executer::exeModifyACL(path, user_permission);
  formatPrint(command_no, "Y", comm, SUCCEED);
}
/*
 * ----------------END Permission Checking Public Interface----------------
 */
