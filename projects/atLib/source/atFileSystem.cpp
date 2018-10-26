#include "atFileSystem.h"
#include <direct.h>

static bool _CreateFoldersRecursive(const atString &done, const atString &remaining)
{
  if (remaining.length() == 0)
    return true;
  int64_t nextSlash = remaining.find_first_of('/');
  atString nextFolder = remaining.substr(0, nextSlash);
  int64_t res = _mkdir(done + nextFolder);
  if (res < 0 && errno != EEXIST) 
    return false;
  return nextSlash != -1 ? _CreateFoldersRecursive(done + nextFolder + "/", remaining.substr(nextSlash, -1)) : true;
}

atFileSystem::atFileSystem() {}
bool atFileSystem::CreateFolders(const atString &path) { return _CreateFoldersRecursive("", atFilename(path).Path()); }
