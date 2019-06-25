
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

#ifndef atFileCommon_h__
#define atFileCommon_h__

#include "atFilename.h"

enum atFileMode
{
  atFM_None = 0,
  atFM_Read = 1,
  atFM_Write = 1 << 1,
  atFM_Append = 1 << 2,
  atFM_ReadWrite = atFM_Read | atFM_Write,
  atFM_Binary = 1 << 5,
  atFM_ReadBinary = atFM_Read | atFM_Binary,
  atFM_WriteBinary = atFM_Write | atFM_Binary,
  atFM_ReadWriteBinary = atFM_ReadWrite | atFM_Binary,
};

enum atFileExtension
{
  atFE_Txt,
  atFE_Obj,
  atFE_Atm,
  atFE_Unknown
};

class atFileCommon
{
public:
  atFileCommon() = delete;

  static const char* FileMode(const int64_t mode);
  static const char* FileExtension(const atFileExtension ext);
  static atFileExtension FileExtension(const char *ext);
};

#endif // atFileCommon_h__
