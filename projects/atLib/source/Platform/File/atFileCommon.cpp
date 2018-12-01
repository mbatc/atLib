
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

#include "atFileCommon.h"

const char* atFileCommon::FileMode(const int64_t mode)
{
  bool binary = (mode & atFM_Binary) > 0;
  switch (mode & (~atFM_Binary))
  {
  case atFM_ReadWrite: return binary ? "rb+" : "r+";
  case atFM_Read: return binary ? "rb" : "r";
  case atFM_Write: return binary ? "wb" : "w";
  case atFM_Append: return binary ? "ab" : "a";
  default: return binary ? "rb" : "r";
  };
}

const char* atFileCommon::FileExtension(const atFileExtension ext)
{
  switch (ext)
  {
  case atFE_Obj: return "obj";
  case atFE_Txt: return "txt";
  case atFE_Atm: return "atm";
  case atFE_Unknown: default: return "";
  }
}

atFileExtension atFileCommon::FileExtension(const char* ext)
{
  if (atString::compare(ext, FileExtension(atFE_Obj), atSCO_None)) return atFE_Obj;
  if (atString::compare(ext, FileExtension(atFE_Txt), atSCO_None)) return atFE_Txt;
  if (atString::compare(ext, FileExtension(atFE_Atm), atSCO_None)) return atFE_Atm;
  return atFE_Unknown;
}
