
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#include "atFileSystem.h"

#ifdef atPLATFORM_WIN32
#include <direct.h>
#include <shlobj.h>

static bool _CreateFoldersRecursive(const atString &done, const atString &remaining)
{
  if (remaining.length() == 0)
    return true;

  int64_t nextSlash = atMax(remaining.find_first_of('/'));
  atString nextFolder = remaining.substr(0, nextSlash);
  int64_t res = _mkdir(done + nextFolder);
  if (res < 0 && errno != EEXIST && *(nextFolder.end() - 1) != ':')
    return false;

  return nextSlash != -1 ? _CreateFoldersRecursive(done + nextFolder + "/", remaining.substr(nextSlash + 1, -1)) : true;
}

static atFilename _GetSystemPath(const int &folderID)
{
  char buffer[MAX_PATH] = { 0 };
  SHGetSpecialFolderPathA(0, buffer, folderID, false);
  return buffer;
}

atFileSystem::atFileSystem() {}
bool atFileSystem::CreateFolders(const atFilename &path) { return _CreateFoldersRecursive("", path.Path()); }

static atVector<atFileSystem::FileInfo> _EnumerateFolder(const atFilename &path, const bool &findFiles, const bool &findFolders)
{
  atVector<atFileSystem::FileInfo> fileInfo;

  // Find the first file in the directory.
  WIN32_FIND_DATA ffd;
  LARGE_INTEGER filesize;
  HANDLE hFind = FindFirstFile(path.Path().replace("/", "\\") + "\\*", &ffd);

  if (INVALID_HANDLE_VALUE == hFind)
    return{};

  // List all the files in the directory with some info about them.

  do
  {
    atFileSystem::FileInfo newInfo;
    newInfo.isFolder = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0;
    if ((newInfo.isFolder && findFolders) || (!newInfo.isFolder && findFiles))
    {
      if (!newInfo.isFolder)
      {
        filesize.LowPart = ffd.nFileSizeLow;
        filesize.HighPart = ffd.nFileSizeHigh;
        newInfo.size = filesize.QuadPart;
      }

      newInfo.path = path.Path() + "/" + ffd.cFileName;
      fileInfo.push_back(newInfo);
    }
  } while (FindNextFile(hFind, &ffd) != 0);

  FindClose(hFind);
  return fileInfo;
}

atVector<atFileSystem::FileInfo> atFileSystem::EnumerateFiles(const atFilename &path) { return _EnumerateFolder(path, true, false); }
atVector<atFileSystem::FileInfo> atFileSystem::EnumerateFolders(const atFilename &path) { return _EnumerateFolder(path, false, true); }
atVector<atFileSystem::FileInfo> atFileSystem::Enumerate(const atFilename &path) { return _EnumerateFolder(path, true, true); }

atFilename atFileSystem::GetDirectory_AppData() { return _GetSystemPath(CSIDL_APPDATA); }
atFilename atFileSystem::GetDirectory_AppData_Local() { return _GetSystemPath(CSIDL_LOCAL_APPDATA); }
atFilename atFileSystem::GetDirectory_Windows() { return _GetSystemPath(CSIDL_WINDOWS); }
atFilename atFileSystem::GetDirectory_Desktop() { return _GetSystemPath(CSIDL_DESKTOP); }
atFilename atFileSystem::GetDirectory_Documents() { return _GetSystemPath(CSIDL_MYDOCUMENTS); }
atFilename atFileSystem::GetDirectory_Fonts() { return _GetSystemPath(CSIDL_FONTS); }
atFilename atFileSystem::GetDirectory_History() { return _GetSystemPath(CSIDL_HISTORY); }
atFilename atFileSystem::GetDirectory_Recents() { return _GetSystemPath(CSIDL_RECENT); }
atFilename atFileSystem::GetDirectory_StartMenu() { return _GetSystemPath(CSIDL_STARTMENU); }
atFilename atFileSystem::GetDirectory_ProgramFiles() { return _GetSystemPath(CSIDL_PROGRAM_FILES); }
atFilename atFileSystem::GetDirectory_ProgramFiles86() { return _GetSystemPath(CSIDL_PROGRAM_FILESX86); }

#endif
