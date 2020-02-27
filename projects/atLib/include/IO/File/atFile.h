
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

#ifndef _atFile_h__
#define _atFile_h__

#include "atFileInfo.h"
#include "atReadStream.h"
#include "atWriteStream.h"

class atFile : public atReadStream, public atWriteStream
{
public:
  atFile();
  atFile(const atFilename &file, const atFileMode mode);
  atFile(const atFile &copy);
  atFile(atFile &&move);

  ~atFile();

  // Open [file] in the specified [mode]
  bool Open(const atFilename &file, const int64_t mode = atFM_ReadWriteBinary);

  // Close the file previously opened with Open()
  // Returns false if no file has been opened
  // Returns true on success
  bool Close();

  // Seek to the specified byte in a file
  // Returns the current seek location
  // Returns atFS_Invalid if [loc] is invalid
  bool Seek(const int64_t loc, const atSeekOrigin origin = atSO_Start) override;

  // Returns the current position in the file
  int64_t Tell() const override;

  // Writes pData to the file
  int64_t Write(const void *pData, const int64_t len) override;

  // Writes pData to the file
  int64_t WriteText(const atString &text);

  atString ReadText();
  static atString ReadText(const atFilename &filename);

  static int64_t WriteFile(const atFilename &filename, const void *pData, const int64_t &len);
  static int64_t WriteTextFile(const atFilename &filename, const atString &content);

  bool Flush();

  // Read data into pBuffer. 
  // Returns the number of bytes read
  int64_t Read(void *pBuffer, const int64_t size);

  const atFileInfo &Info();

  int64_t GetMode();
  bool IsOpen() const;

  template<typename T> int64_t Read(T *pData, const int64_t count = 1);
  template<typename T> int64_t Write(const T *pData, const int64_t count = 1);
  template<typename T> int64_t Write(const T &data);

  static bool Exists(const atFilename &fn);
  static bool Create(const atFilename &fn);
  static bool Delete(const atFilename &fn);
  static bool Copy(const atFilename &fn);
  static bool Move(const atFilename &fn);

protected:
  FILE *m_pFile;
  int64_t m_mode;
  int64_t m_pos;
  atFilename m_fn;
  atFileInfo m_info;
};

template<typename T> int64_t atFile::Read(T *pData, const int64_t count) { return atStreamRead(this, (T*)pData, count); }
template<typename T> int64_t atFile::Write(const T *pData, const int64_t count) { return atStreamWrite(this, (const T *)pData, count); }
template<typename T> int64_t atFile::Write(const T &data) { return Write(&data, 1); }

#endif
