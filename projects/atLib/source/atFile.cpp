
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

#include "atFile.h"
#include <Windows.h>

atFile::atFile() : m_pFile(nullptr) { Close(); }
atFile::atFile(const atFilename &file, const atFileMode mode) : m_pFile(nullptr), m_mode(atFM_None) { Open(file, mode); }
atFile::atFile(const atFile &copy) : m_pFile(nullptr) { Open(copy.m_fn, copy.m_mode); }
atFile::~atFile() { Close(); }

atFile::atFile(atFile &&move)
  : m_pFile(move.m_pFile)
  , m_mode(move.m_mode)
  , m_fn(move.m_fn)
{
  move.m_pFile = nullptr;
  move.m_mode = atFM_None;
  move.m_fn = "";
}

bool atFile::Open(const atFilename &file, const atFileMode mode)
{
  if (file == m_fn)
    return file != "";
  Close();
  fopen_s(&m_pFile, file.c_str(), atFileCommon::FileMode(mode));
  if (m_pFile)
  {
    m_mode = mode;
    m_fn = file;
    m_info.SetFile(file);
  }

  return m_pFile != nullptr;
}

bool atFile::Close()
{
  if (!m_pFile)
    return false;
  Flush();
  fclose(m_pFile);
  m_pFile = nullptr;
  m_fn = "";
  m_mode = atFM_None; 
  m_info = atFileInfo();
  return true;
}

int64_t atFile::Seek(const int64_t loc)
{
  atUnused(loc);
  return int64_t();
}

int64_t atFile::Write(void *pData, const int64_t len)
{
  if (!IsOpen())
    return 0;
  return fwrite(pData, sizeof(uint8_t), len / sizeof(uint8_t), m_pFile);
}


bool atFile::Flush()
{
  if (!IsOpen())
    return false;
  return fflush(m_pFile) == 0;
}

int64_t atFile::Read(void *pBuffer, const int64_t size)
{
  if (!IsOpen())
    return 0;
  return fread_s(pBuffer, (size_t)size, sizeof(uint8_t), size / sizeof(uint8_t), m_pFile);
}

bool atFile::Exists(const atFilename &fn)
{ 
  bool invalid = GetFileAttributesA(fn.c_str()) == INVALID_FILE_ATTRIBUTES;
  DWORD errorCode = GetLastError();
  return  !(invalid && (errorCode == ERROR_FILE_NOT_FOUND || errorCode == ERROR_PATH_NOT_FOUND || 
      errorCode == ERROR_INVALID_NAME || errorCode == ERROR_INVALID_DRIVE || 
      errorCode == ERROR_NOT_READY || errorCode == ERROR_INVALID_PARAMETER || 
      errorCode == ERROR_BAD_PATHNAME || errorCode == ERROR_BAD_NETPATH));
}

bool atFile::Create(const atFilename &fn) { return atFile(fn, atFM_Append).IsOpen(); }
bool atFile::Copy(const atFilename &fn) { atUnused(fn); return false; }
bool atFile::Move(const atFilename &fn) { atUnused(fn); return false; }
const atFileInfo& atFile::Info() { return m_info; }
atFileMode atFile::GetMode() { return m_mode; }
bool atFile::IsOpen() { return m_mode != atFM_None; }
bool atFile::Delete(const atFilename &fn) { return remove(fn.Path().c_str()) == 0; }
