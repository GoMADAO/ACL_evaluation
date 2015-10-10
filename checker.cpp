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

//  public interface of read with format output
void Checker::read(std::string path, std::string user_group) {
  ACLFile *parent = getParent(path, fs->root);

  if (parent == NULL) {
    // format print not find path to the file
    return;
  }
  
  std::vector<std::string> u_g;
  split(u_g, user_group, ".");

  
  if (!canReadPath(path, u_g[0], u_g[1])) {
    // format print can not read the path

    return;
  }

  std::string file_name = SplitFilename(path);
  if (parent->file_link.find(file_name) == parent->file_link.end()) {
    //  format print not found file
    return;
  }

  if (!canRead(u_g[0], u_g[1], parent->file_link.at(file_name))) {
    //  format print not read permission on current file
    std::cout << user_group << " has no read permission on file " << file_name
              << std::endl;
    return;
  }
  std::cout << user_group << " has read permission on file " << file_name
            << std::endl;

  return;
}


void Checker::write(std::string path, std::string user_group){
  ACLFile *parent = getParent(path, fs->root);
  
  if (parent == NULL) {
    // format print not find path to the file
    return;
  }
  
  std::vector<std::string> u_g;
  split(u_g, user_group, ".");
  
  if (!canReadPath(path, u_g[0], u_g[1])) {
    // format print can not read the path
    
    return;
  }

  std::string file_name = SplitFilename(path);
  if (parent->file_link.find(file_name) == parent->file_link.end()) {
    //  format print not found file
    return;
  }
  
  if (!canWrite(u_g[0], u_g[1], parent->file_link.at(file_name))) {
    //  format print not write permission on current file
    std::cout << user_group << " has no write permission on file " << file_name
    << std::endl;
    return;
  }
  std::cout << user_group << " has write permission on file " << file_name
  << std::endl;
  
}

void Checker::creadel(std::string path, std::string user_group){
  ACLFile *parent = getParent(path, fs->root);
  
  if (parent == NULL) {
    // format print not find path to the file
    return;
  }
  
  std::vector<std::string> u_g;
  split(u_g, user_group, ".");
  
  if (!canReadPath(path, u_g[0], u_g[1])) {
    // format print can not read the path
    
    return;
  }
  
  if (!canWrite(u_g[0], u_g[1], parent)) {
    //  format print not write permission on current file
    std::cout << user_group << " has no create or delete permission on file " << parent->file_name
    << std::endl;
    return;
  }
  
  std::cout << user_group << " has create or delete permission on file " << parent->file_name << std::endl;
  
  
}



