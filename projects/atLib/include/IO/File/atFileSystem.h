
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

#ifndef atFileSystem_h__
#define atFileSystem_h__

#include "atFileCommon.h"
#include "atFilename.h"

class atFileSystem
{
public:
  atFileSystem();

  struct FileInfo
  {
    bool isFolder;
    int64_t size;
    atFilename path;
  };

  static bool CreateFolders(const atFilename &path);
  static atVector<FileInfo> EnumerateFiles(const atFilename &path);
  static atVector<FileInfo> EnumerateFolders(const atFilename &path);
  static atVector<FileInfo> Enumerate(const atFilename &path);

  static atFilename GetDirectory_AppData();
  static atFilename GetDirectory_AppData_Local();
  static atFilename GetDirectory_Windows();
  static atFilename GetDirectory_Desktop();
  static atFilename GetDirectory_Documents();
  static atFilename GetDirectory_Fonts();
  static atFilename GetDirectory_History();
  static atFilename GetDirectory_Recents();
  static atFilename GetDirectory_StartMenu();
  static atFilename GetDirectory_ProgramFiles();
  static atFilename GetDirectory_ProgramFiles86();
};

#endif // atFileSystem_h__
