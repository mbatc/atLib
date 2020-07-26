
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

#ifndef _atFilename_h__
#define _atFilename_h__

#include "atString.h"

class atFilename
{
public:
  atFilename();
  atFilename(const char *path);
  atFilename(const atString &path);
  atFilename(const atFilename &copy);
  atFilename(atFilename &&move);
  
  atString Path(const bool withExtension = true) const;
  atString Name(const bool withExtension = true) const;
  atString Extension() const;
  atString Directory() const;

  atFilename ResolveFullPath() const;
  static atFilename ResolveFullPath(const atFilename &path);

  // Not Implemented!
  atFilename ResolveRelativePath(const atFilename &to) const;
  static atFilename ResolveRelativePath(const atFilename &to, const atFilename &from);

  void assign(const atString &path);

  const char* c_str() const;

  bool operator==(const atFilename &fn) const;
  bool operator!=(const atFilename &fn) const;
  bool operator==(const atString &fn) const;
  bool operator!=(const atString &fn) const;
  bool operator==(const char *fn) const;
  bool operator!=(const char *fn) const;

  atFilename operator=(const atFilename &fn);
  atFilename operator=(const atString &fn);
  atFilename operator=(const char *fn);

  friend int64_t atStreamRead(atReadStream *pStream, atFilename *pData, const int64_t count);
  friend int64_t atStreamWrite(atWriteStream *pStream, const atFilename *pData, const int64_t count);

protected:
  atString m_fullpath;
  atString m_name;
  atString m_extension;
  atString m_directory;
};

template<> atFilename atFromString<atFilename>(const atString &str);

#endif