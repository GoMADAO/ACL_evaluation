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

  cout << fs->root->file_link.at("/home")->file_name << endl;
  cout << fs->root->file_link.at("/tmp")->file_name << endl;
}


void testSplit(){
  std::string s = "smb.faculty /home/smb";
  std::vector <std::string> fields;
  split(fields, s, " ", split::no_empties );
  
  for(size_t n =0; n<fields.size(); ++n){
    std::cout<< fields[n]<<std::endl;
  }
}

void testReadPermssion(){
  Checker::read("/home/sal", "sal.faculty");
  Checker::read("/home/sal", "smb.columbia");
}

void testWritePermssion(){
  Checker::write("/home/sal", "sal.faculty");
  Checker::write("/home/sal", "smb.columbia");
  Checker::creadel("/home/sal/a", "sal.faculty");
}


int main(int argc, const char *argv[]) {

  // environment
  fs = new FileSystem;
  
  // read a line into buff
  string buff;
  
  vector<string> tokens;
  vector<string> user_group_list;
  string file_name;
  
  //  user definition section
  while (getline(cin, buff) && buff!=".") {
    //  split line into tokens and checke the size
    split(tokens, buff, " ", split::no_empties);
    
    //  tokens size = 2, then meet a new valid file permission description
    //  e.g. smb.faculty /home/smb
    if(tokens.size()>1){
      
      if(!user_group_list.empty()){
        // build file system
        fs->addUserGroup(&user_group_list);
        fs->addACLFile(&user_group_list, file_name);
        user_group_list.clear();
      }
      user_group_list.push_back(tokens[0]);
      file_name = tokens[1];
    
    }else{
      //  not finish file permission description
      //  e.g. smb.columbia
      user_group_list.push_back(tokens[0]);
    }
  }
  
  // rest of user definition
  fs->addUserGroup(&user_group_list);
  fs->addACLFile(&user_group_list, file_name);
  user_group_list.clear();
  
  
  
  /*delete me*/
  fs->showFilePermission();
  testWritePermssion();
  
  // file operation section
//  while(getline(cin, buff)){
//    cout<< buff <<endl;
//  }

  
  
  delete fs;
  return 0;
}
