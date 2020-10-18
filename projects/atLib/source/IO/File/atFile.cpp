
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

#include "atFile.h"
#include "atPlatform.h"
#include "atFileSystem.h"

static void _OpenFile(FILE **ppFile, const char *pFilename, const int64_t &fm)
{
#ifdef atPLATFORM_WIN32
  fopen_s(ppFile, pFilename, atFileCommon::FileMode(fm));
#else
  *ppFile = fopen(pFilename, atFileCommon::FileMode(fm));
#endif
}

static void _CloseFile(FILE **ppFile)
{
  if (*ppFile)
    fclose(*ppFile);
  *ppFile = 0;
}

static int64_t _WriteFile(const void *pData, size_t len, FILE *pFile)
{
  return fwrite(pData, len, 1, pFile);
}

static int64_t _ReadFile(void *pBuffer, size_t len, FILE *pFile)
{
#ifdef atPLATFORM_WIN32
  return fread_s(pBuffer, (size_t)len, (size_t)len, 1, pFile);
#else
  return fread(pBuffer, (size_t)len, 1, pFile);
#endif
}

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

bool atFile::Open(const atFilename &file, const int64_t mode)
{
  if (file == m_fn)
    return file != "";
  Close();
  _OpenFile(&m_pFile, file.c_str(), mode);
  if (m_pFile)
  {
    m_mode = mode;
    m_fn = file;
    m_info.SetFile(file);
    m_pos = Tell();
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

bool atFile::Seek(const int64_t loc, const atSeekOrigin origin)
{
  if (!m_pFile)
    return false;

  switch (origin)
  {
  case atSO_Current: m_pos = m_pos + loc; break;
  case atSO_Start: m_pos = loc; break;
  case atSO_End: m_pos = m_info.Size() - loc; break;
  }

  return fseek(m_pFile, (long)loc, origin) == 0;
}

int64_t atFile::Tell() const
{
  return IsOpen() ? ftell(m_pFile) : 0;
}

int64_t atFile::Write(const void *pData, const int64_t len)
{
  if (!IsOpen())
    return 0;
  int64_t bytesWritten = _WriteFile(pData, (size_t)len, m_pFile);
  m_pos += bytesWritten;
  return bytesWritten * len;
}

atString atFile::ReadText()
{
  if (!IsOpen() || m_info.Size() == 0)
    return "";
  atVector<char> data(m_info.Size(), 0);
  Read(data.data(), data.size());
  return data.data();
}

atString atFile::ReadText(const atFilename &filename)
{
  atFile file;
  atString ret;
  if (file.Open(filename, atFM_Read))
    ret = file.ReadText();
  return ret;
}

int64_t atFile::WriteFile(const atFilename &filename, const void *pData, const int64_t &len)
{
  atFile output;
  if (!output.Open(filename, atFM_WriteBinary))
    return 0;
  return output.Write(pData, len);
}

int64_t atFile::WriteTextFile(const atFilename &filename, const atString &content)
{
  return WriteFile(filename, content.c_str(), content.length());
}

bool atFile::Flush()
{
  return IsOpen() ? fflush(m_pFile) == 0 : false;
}

int64_t atFile::Read(void *pBuffer, const int64_t size)
{
  if (!IsOpen() || size == 0)
    return 0;
  if (_ReadFile(pBuffer, (size_t)size, m_pFile) != 1)
    if (!feof(m_pFile))
      atAssert(false, "Read failed with error: " + atString(ferror(m_pFile)));
  m_pos += size;
  return size;
}

atFilename atFile::Find(const atFilename &fn, bool *pResult)
{
  if (pResult)
    *pResult = true;

  if (Exists(fn))
    return fn;

  // Iterate sub folders and check if the file exists in them
  for (const atFileSystem::FileInfo &subFolder : atFileSystem::EnumerateFolders(fn.Directory()))
  {
    atFilename path = subFolder.path.Path() + "/" + fn.Name();
    if (Exists(path))
      return path;
  }

  if (pResult)
    *pResult = false;
  return fn;
}

bool atFile::Exists(const atFilename &fn)
{
#ifdef atPLATFORM_WIN32
  bool invalid = GetFileAttributesA(fn.c_str()) == INVALID_FILE_ATTRIBUTES;
  DWORD errorCode = GetLastError();
  return  !(invalid && (errorCode == ERROR_FILE_NOT_FOUND || errorCode == ERROR_PATH_NOT_FOUND || 
      errorCode == ERROR_INVALID_NAME || errorCode == ERROR_INVALID_DRIVE || 
      errorCode == ERROR_NOT_READY || errorCode == ERROR_INVALID_PARAMETER || 
      errorCode == ERROR_BAD_PATHNAME || errorCode == ERROR_BAD_NETPATH));
#else
  int result = access(fn.c_str(), X_OK);
  return result == 0 || result == EACCES || result == EROFS;
#endif
}

int64_t atFile::WriteText(const atString &text) { return Write(text.c_str(), text.length()); }
bool atFile::Create(const atFilename &fn) { return atFile(fn, atFM_Append).IsOpen(); }
bool atFile::Copy(const atFilename &fn) { atUnused(fn); return false; }
bool atFile::Move(const atFilename &fn) { atUnused(fn); return false; }
const atFileInfo& atFile::Info() { return m_info; }
int64_t atFile::GetMode() { return m_mode; }
bool atFile::IsOpen() const { return m_pFile && m_mode != atFM_None; }
bool atFile::Delete(const atFilename &fn) { return remove(fn.Path().c_str()) == 0; }
