//
//  filesystem.cpp
//  ACL_evaluation
//
//  Created by apple  on 10/8/15.
//  Copyright (c) 2015 apple . All rights reserved.
//

#include "filesystem.h"

FileSystem::FileSystem() {
  // root of the filesystem
  root = new ACLFile("/");
  root->file_permission.push_back(acl_pair("*.*", 2));

  // home directory
  ACLFile *home = new ACLFile("/home");
  home->file_permission.push_back(acl_pair("*.*", 2));
  root->file_link.insert(std::make_pair("/home", home));

  // tmp directory
  ACLFile *tmp = new ACLFile("/tmp");
  tmp->file_permission.push_back(acl_pair("*.*", 3));
  root->file_link.insert(std::make_pair("/tmp", tmp));
}

FileSystem::~FileSystem() {
  for (std::map<std::string, ACLFile *>::iterator i = root->file_link.begin();
       i != root->file_link.end(); ++i) {
    // std::cout<<i->first<<std::endl;
    delete root->file_link.at(i->first);
  }
  root->file_link.clear();
  root->file_permission.clear();
  delete root;
  users.clear();
  groups.clear();
}

void FileSystem::addUserGroup(std::vector<std::string> *user_group) {
  std::vector<std::string> lines = *user_group;
  std::string line;
  std::vector<std::string> temp;
  std::map<std::string, std::set<std::string>>::iterator u_it;
  std::map<std::string, std::set<std::string>>::iterator g_it;
  for (std::vector<std::string>::iterator it = lines.begin(); it < lines.end();
       ++it) {

    //  split the format "smb.faculty"
    split(temp, *it, ".", split::no_empties);

    //  add groups into users
    u_it = users.find(temp[0]);

    if (u_it == users.end()) {
      std::set<std::string> u_groups;
      u_groups.insert(temp[1]);
      users.insert(std::make_pair(temp[0], u_groups));
    } else {
      users.at(temp[0]).insert(temp[1]);
    }

    //  add users into groups
    g_it = groups.find(temp[1]);

    if (g_it == groups.end()) {
      std::set<std::string> g_users;
      g_users.insert(temp[0]);
      groups.insert(std::make_pair(temp[1], g_users));
    } else {
      groups.at(temp[1]).insert(temp[0]);
    }
  }
  return;
}

void FileSystem::addACLFile(std::vector<std::string> *user_group,
                            std::string file_path) {

  std::vector<std::string> lines = *user_group;

  /*
   create an ACLfile and set default permission
   with user.group rw
   *.*       r
   */
  ACLFile *parent = getParent(file_path, root);

  ACLFile *cur;
  std::string name = SplitFilename(file_path);

  //  EXIST file add more permission
  if (parent->file_link.find(name) != parent->file_link.end()) {
    cur = parent->file_link.at(name);
    cur->file_permission.pop_back();

    for (std::vector<std::string>::iterator it = user_group->begin();
         it != user_group->end(); ++it) {
      cur->file_permission.push_back(acl_pair(*it, 3));
    }
    cur->file_permission.push_back(acl_pair("*.*", 2));
    return;
  }

  cur = new ACLFile(name, *user_group);
  parent->file_link.insert(std::make_pair(name, cur));

  return;
}

/*-------------------------------------------*/

void FileSystem::showUserGroup() {
  std::string user_name;
  std::string group_name;
  for (std::map<std::string, std::set<std::string>>::iterator it =
           users.begin();
       it != users.end(); ++it) {
    user_name = it->first;
    std::cout << "USER :" << user_name << std::endl << " belongs to GROUP: ";
    for (std::set<std::string>::iterator itt = it->second.begin();
         itt != it->second.end(); ++itt) {
      group_name = *itt;
      std::cout << group_name << " ";
    }
    std::cout << std::endl;
  }

  for (std::map<std::string, std::set<std::string>>::iterator it =
           groups.begin();
       it != groups.end(); ++it) {
    group_name = it->first;
    std::cout << "GROUP :" << group_name << std::endl << " has USER: ";
    for (std::set<std::string>::iterator itt = it->second.begin();
         itt != it->second.end(); ++itt) {
      user_name = *itt;
      std::cout << user_name << " ";
    }
    std::cout << std::endl;
  }
}

void showACLpair(std::vector<acl_pair> file_permission) {
  for (size_t n = 0; n < file_permission.size(); ++n)
    std::cout << "user: " << file_permission[n].first
              << " permission:" << file_permission[n].second << std::endl;
}

void showFileHelper(ACLFile *p) {
  std::cout << p->file_name << std::endl;
  showACLpair(p->file_permission);
  if (p->file_link.size() == 0) {
    return;
  }
  for (std::map<std::string, ACLFile *>::iterator it = p->file_link.begin();
       it != p->file_link.end(); ++it) {
    showFileHelper(it->second);
  }
}

void FileSystem::showFilePermission() { showFileHelper(root); }
