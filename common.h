//
//  common.h
//  ACL_evaluation
//
//  Created by apple  on 10/10/15.
//  Copyright (c) 2015 apple . All rights reserved.
//

#ifndef ACL_evaluation_common_h
#define ACL_evaluation_common_h
#include "filesystem.h"

#define INVALIDINPUT "ERROR: Not a valid input: mulitple space"
#define INVALIDUSERGROUP                                                       \
  "ERROR: Not a valid user.group: all character should be lowcase and "        \
  "contains only one period"
#define INVALIDFILENAME "ERROR: Not a valid file name"
#define USERNOFILE "ERROR: User does not have any file name"
#define INVALIDPATH                                                            \
  "ERROR: The file already contains other file, cannot modify ACL"

#define SUCCEED ""
#define NOTFOUNDOPERATION                                                      \
  "ERROR: OPERATION is not defined, make sure use capital letter"
#define FAILEDFINDPATH "ERROR: Failed to FIND the path to the file"
#define FAILEDREADPATH                                                         \
  "NO_PERMISSION: No READ permssion on the path to the file"
#define FILENOTEXIST "ERROR: File does NOT EXIST"
#define FILEALREADYEXIST "ERROR: File already EXIST"
#define NOREADPERMISSION "NO_PERMISSION: No READ permission on the file"
#define NOWRITEPERMISSION "NO_PERMISSION: No WRITE permission on the file"
#define NOWRITEPERMISSIONPARENT                                                \
  "NO_PERMISSION: No WRITE permission on its previous component"
#define EMPTYACL "ERROR: Not allow DELETE ACL content"
#define USERNOTEXISTS "ERROR: User does not exists"
#define GROUPNOTEXISTS "ERROR: Group does not exists"

extern FileSystem *fs;

#endif
