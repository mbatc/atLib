
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

#include "atFileInfo.h"

bool atFileInfo::SetFile(const atFilename &file)
{
  m_exists = stat(file.c_str(), &m_data) == 0;
  return m_exists;
}

atFileInfo::atFileInfo() : m_exists(false) {}
atFileInfo::atFileInfo(const atFilename &file) { SetFile(file); }
bool atFileInfo::IsDirectory() const { return (m_data.st_mode & _S_IFDIR) > 0; }
int64_t atFileInfo::Size() const { return m_data.st_size; }
int64_t atFileInfo::DriveNo() const { return m_data.st_dev; }
atDateTime atFileInfo::Created() const { return atDateTime(m_data.st_ctime); }
atDateTime atFileInfo::LastAccessed() const { return atDateTime(m_data.st_atime); }
atDateTime atFileInfo::LastModified() const { return atDateTime(m_data.st_mtime); }
const atFilename& atFileInfo::Filename() const { return m_fn; }
bool atFileInfo::IsDirectory(const atFilename &file) { return atFileInfo(file).IsDirectory(); }
int64_t atFileInfo::Size(const atFilename &file) { return atFileInfo(file).Size(); }
int64_t atFileInfo::DriveNo(const atFilename &file) { return atFileInfo(file).DriveNo(); }
atDateTime atFileInfo::Created(const atFilename &file) { return atFileInfo(file).Created(); }
atDateTime atFileInfo::LastAccessed(const atFilename &file) { return atFileInfo(file).LastAccessed(); }
atDateTime atFileInfo::LastModified(const atFilename &file) { return atFileInfo(file).LastModified(); }
