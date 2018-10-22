
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
  atFilenameBasic(const atStringBasic<c> &path = "");
  atFilenameBasic(const atFilenameBasic &copy);
  atFilenameBasic(atFilenameBasic &&move);
  
  atStringBasic<c> Path(const bool withExtension = true) const;
  atStringBasic<c> Name(const bool withExtension = true) const;
  atStringBasic<c> Extension() const;
  atStringBasic<c> Directory() const;

  void assign(const atStringBasic<c> &path);

  const char* c_str() const;

  bool operator==(const atFilenameBasic &fn) const;
  bool operator!=(const atFilenameBasic &fn) const;
  bool operator==(const atStringBasic<c> &fn) const;
  bool operator!=(const atStringBasic<c> &fn) const;

  atFilenameBasic operator=(const atFilenameBasic &fn);
  atFilenameBasic operator=(const atStringBasic<c> &fn);
protected:
  atStringBasic<c> m_fullpath;
  atStringBasic<c> m_name;
  atStringBasic<c> m_extension;
  atStringBasic<c> m_directory;
};

typedef atFilenameBasic<char> atFilename;
typedef atFilenameBasic<wchar_t> atWideFilename;


template <typename T> _atStreamRead(atFilenameBasic<T>)
{
  atString path;
  int64_t ret = atStreamRead(pStream, &path);
  pData->assign(path);
  return ret;
}

template <typename T> _atStreamWrite(atFilenameBasic<T>) { return atStreamWrite(pStream, data.Path()); }


#include "atFilename.inl"
#endif