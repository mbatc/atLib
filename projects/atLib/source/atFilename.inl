
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

template <typename c> atFilenameBasic<c>::atFilenameBasic(atFilenameBasic &&move)
  : m_fullpath(move.m_fullpath)
  , m_directory(move.m_directory)
  , m_extension(move.m_extension)
  , m_name(move.m_name)
{}

template <typename c> void atFilenameBasic<c>::assign(const atStringBasic<c> &path)
{
  m_fullpath = path;
  m_fullpath = m_fullpath.replace('\\', '/');

  const int64_t lastDot = m_fullpath.find_last('.');
  const int64_t lastSlash = m_fullpath.find_last('/');
  if(lastDot > 0 && lastDot > lastSlash)
    m_extension = m_fullpath.substr(lastDot + 1, m_fullpath.length());
  m_name = m_fullpath.substr(lastSlash + 1, m_fullpath.length() - (m_extension.length() > 0 ? 1 : 0) - m_extension.length());
  m_directory = m_fullpath.substr(0, lastSlash);
}

template <typename c> atFilenameBasic<c> atFilenameBasic<c>::operator=(const atFilenameBasic<c> &fn) { assign(fn.m_fullpath);  return *this; }
template <typename c> atFilenameBasic<c> atFilenameBasic<c>::operator=(const atStringBasic<c> &fn) { assign(fn); return *this; }
template <typename c> bool atFilenameBasic<c>::operator==(const atFilenameBasic<c> &fn) const { return m_fullpath == fn.m_fullpath; }
template <typename c> bool atFilenameBasic<c>::operator!=(const atFilenameBasic<c> &fn) const { return !(*this == fn); }
template <typename c> bool atFilenameBasic<c>::operator==(const atStringBasic<c> &fn) const { return m_fullpath == fn.replace('\\', '/'); }
template <typename c> bool atFilenameBasic<c>::operator!=(const atStringBasic<c> &fn) const { return !(*this == fn); }
template <typename c> const char * atFilenameBasic<c>::c_str() const { return m_fullpath.c_str(); }
template <typename c> atFilenameBasic<c>::atFilenameBasic(const atStringBasic<c> &path) { assign(path); }
template <typename c> atFilenameBasic<c>::atFilenameBasic(const atFilenameBasic<c> &copy) { assign(copy.m_fullpath); }
template <typename c> atStringBasic<c> atFilenameBasic<c>::Path(const bool withExtension) const { return withExtension ? m_fullpath : m_directory + "/" + m_name; }
template <typename c> atStringBasic<c> atFilenameBasic<c>::Name(const bool withExtension) const { return withExtension ? m_name + "." + m_extension : m_name; }
template <typename c> atStringBasic<c> atFilenameBasic<c>::Extension() const { return m_extension; }
template <typename c> atStringBasic<c> atFilenameBasic<c>::Directory() const { return m_directory; }