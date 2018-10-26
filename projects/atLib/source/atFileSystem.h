#ifndef atFileSystem_h__
#define atFileSystem_h__

#include "atFileCommon.h"

class atFileSystem
{
public:
  atFileSystem();
  
  static bool CreateFolders(const atString &path);
};

#endif // atFileSystem_h__
