
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

#include "atFilename.h"
#include <windows.h>

atFilename::atFilename(atFilename &&move)
  : m_fullpath(move.m_fullpath)
  , m_directory(move.m_directory)
  , m_extension(move.m_extension)
  , m_name(move.m_name)
{}

void atFilename::assign(const atString &path)
{
  m_fullpath = path;
  m_fullpath = m_fullpath.replace('\\', '/');
  m_fullpath = m_fullpath.replace("//", "/");

  const int64_t lastDot = m_fullpath.find_last('.');
  const int64_t lastSlash = m_fullpath.find_last('/');
  if (lastDot > 0 && lastDot > lastSlash)
    m_extension = m_fullpath.substr(lastDot + 1, m_fullpath.length());
  m_name = m_fullpath.substr(lastSlash + 1, m_fullpath.length() - (m_extension.length() > 0 ? 1 : 0) - m_extension.length());
  m_directory = m_fullpath.substr(0, lastSlash);
}

atFilename atFilename::ResolveFullPath(const atFilename &path)
{
  char buffer[MAX_PATH] = { 0 };
  _fullpath(buffer, path.Path(), MAX_PATH);
  return atFilename(buffer);
}

atFilename atFilename::ResolveRelativePath(const atFilename &to, const atFilename &from)
{
  return atFilename();
}

atFilename atFilename::operator=(const atFilename &fn) { assign(fn.m_fullpath);  return *this; }
atFilename atFilename::operator=(const atString &fn) { assign(fn); return *this; }
atFilename atFilename::operator=(const char *fn) { assign(fn);  return *this; }
bool atFilename::operator==(const atFilename &fn) const { return m_fullpath == fn.m_fullpath; }
bool atFilename::operator!=(const atFilename &fn) const { return !(*this == fn); }
bool atFilename::operator==(const atString &fn) const { return m_fullpath == fn.replace('\\', '/'); }
bool atFilename::operator!=(const atString &fn) const { return !(*this == fn); }
bool atFilename::operator==(const char *fn) const { return *this == atString(fn); }
bool atFilename::operator!=(const char *fn) const { return *this != atString(fn); }
const char* atFilename::c_str() const { return m_fullpath.c_str(); }
atFilename::atFilename() : atFilename("") {}
atFilename::atFilename(const char *path) { assign(path); }
atFilename::atFilename(const atString &path) { assign(path); }
atFilename::atFilename(const atFilename &copy) { assign(copy.m_fullpath); }
atString atFilename::Path(const bool withExtension) const { return withExtension ? m_fullpath : m_directory + "/" + m_name; }
atString atFilename::Name(const bool withExtension) const { return withExtension ? m_name + "." + m_extension : m_name; }
atString atFilename::Extension() const { return m_extension; }
atString atFilename::Directory() const { return m_directory; }
atFilename atFilename::ResolveFullPath() const { return ResolveFullPath(*this); }
atFilename atFilename::ResolveRelativePath(const atFilename &to) const { return ResolveRelativePath(to, *this); }

int64_t atStreamRead(atReadStream *pStream, atFilename *pData, const int64_t count)
{
  atString path;
  int64_t ret = 0;
  for (atFilename &fn : atIterate(pData, count))
  {
    ret += atStreamRead(pStream, &path, 1);
    fn.assign(path);
  }
  return ret;
}

int64_t atStreamWrite(atWriteStream *pStream, const atFilename *pData, const int64_t count)
{
  int64_t ret = 0;
  for (const atFilename &fn : atIterate(pData, count))
    ret += atStreamWrite(pStream, &fn.Path(), 1);
  return ret;
}
