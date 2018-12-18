
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
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

template <typename c> class atFilenameBasic
{
public:
  atFilenameBasic(const c *path);
  atFilenameBasic(const atStringBasic<c> &path = "");
  atFilenameBasic(const atFilenameBasic<c> &copy);
  atFilenameBasic(atFilenameBasic<c> &&move);
  
  atStringBasic<c> Path(const bool withExtension = true) const;
  atStringBasic<c> Name(const bool withExtension = true) const;
  atStringBasic<c> Extension() const;
  atStringBasic<c> Directory() const;

  atFilenameBasic<c> ResolveFullPath() const;
  static atFilenameBasic<c> ResolveFullPath(const atFilenameBasic<c> &path);

  // Not Implemented!
  atFilenameBasic<c> ResolveRelativePath(const atFilenameBasic<c> &to) const;
  static atFilenameBasic<c> ResolveRelativePath(const atFilenameBasic<c> &to, const atFilenameBasic<c> &from);

  void assign(const atStringBasic<c> &path);

  const char* c_str() const;

  bool operator==(const atFilenameBasic<c> &fn) const;
  bool operator!=(const atFilenameBasic<c> &fn) const;
  bool operator==(const atStringBasic<c> &fn) const;
  bool operator!=(const atStringBasic<c> &fn) const;
  bool operator==(const char *fn) const;
  bool operator!=(const char *fn) const;

  atFilenameBasic<c> operator=(const atFilenameBasic<c> &fn);
  atFilenameBasic<c> operator=(const atStringBasic<c> &fn);
  atFilenameBasic<c> operator=(const c *fn);

protected:
  atStringBasic<c> m_fullpath;
  atStringBasic<c> m_name;
  atStringBasic<c> m_extension;
  atStringBasic<c> m_directory;
};

typedef atFilenameBasic<char> atFilename;
typedef atFilenameBasic<wchar_t> atWideFilename;

template <> atFilenameBasic<char> atFilenameBasic<char>::ResolveFullPath(const atFilenameBasic<char> &path);

template <typename T> int64_t atStreamRead(atReadStream *pStream, atFilenameBasic<T> *pData, const int64_t count)
{
  atString path;
  int64_t ret = 0;
  for (atFilenameBasic<T> &fn : atIterate(pData, count))
  {
    ret += atStreamRead(pStream, &path, 1);
    fn.assign(path);
  }
  return ret;
}

template <typename T> int64_t atStreamWrite(atWriteStream *pStream, const atFilenameBasic<T> *pData, const int64_t count) 
{
  int64_t ret = 0;
  for(const atFilenameBasic<T> &fn : atIterate(pData, count))
    ret += atStreamWrite(pStream, &fn.Path(), 1);
  return ret;
}

#include "atFilename.inl"
#endif