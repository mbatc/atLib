
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

#ifndef atFileInfo_h__
#define atFileInfo_h__

#include "atFileCommon.h"
#include "atDateTime.h"
#include "atFilename.h"
#include <sys/stat.h>

class atFileInfo
{
public:
  atFileInfo();
  atFileInfo(const atFilename &file);

  bool SetFile(const atFilename &file);

  bool IsDirectory() const;

  int64_t Size() const;
  int64_t DriveNo() const;
  
  atDateTime Created() const;
  atDateTime LastAccessed() const;
  atDateTime LastModified() const;

  const atFilename &Filename() const;

  static bool IsDirectory(const atFilename &file);
  static int64_t Size(const atFilename &file);
  static int64_t DriveNo(const atFilename &file);
  static atDateTime Created(const atFilename &file);
  static atDateTime LastAccessed(const atFilename &file);
  static atDateTime LastModified(const atFilename &file);

protected:
  struct stat m_data;
  atFilename m_fn;
  bool m_exists;
};

#endif // atFileInfo_h__
