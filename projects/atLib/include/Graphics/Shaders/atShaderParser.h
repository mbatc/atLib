
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

#ifndef atShaderParser_h__
#define atShaderParser_h__

#include "atFilename.h"
#include "atHashMap.h"

enum atHSLSBufferType
{
  atHLSLBT_Struct,
  atHLSLBT_Const,
  atHLSLBT_Tex,
  atHLSLBT_Count
};

struct atHLSLBufferDesc
{
  atString name;
                 
  atVector<atString> packingOrder; // the order that variables are packed in the buffer
  atVector<int64_t> packingOffsets; // packingOffsets[i] describes the offset of the variable packingOrder[i]
  atHashMap<atString, atString> semantics; // [Key] == the variable name and the [Value] is the semantic paired with that variable
  atHashMap<atString, atTypeDesc> members; // [Key] == the variable name and the [Value] is the Type Description of the variable
  atHashMap<atString, atHLSLBufferDesc> structs;  // [Key] == the struct name and the [Value] is the Buffer Description of the struct
  atHSLSBufferType type = atHLSLBT_Struct;
  int64_t reg = -1;
  int64_t size = 0; // Size of the struct in bytes
  int64_t actualSize = 0; // Size of the struct in bytes when alignment padding is considered
};

class atShaderParser
{
public:
  atShaderParser() = default;
  atShaderParser(const atFilename &file);
  atShaderParser(const atString &src);
  atShaderParser(const atShaderParser &copy);
  atShaderParser(atShaderParser &&move);

  bool Parse();
  void Clear();

  // [Key] == the register of the cbuffer
  atHashMap<int64_t, atHLSLBufferDesc> m_cbuffers;
  atHashMap<int64_t, atHLSLBufferDesc> m_tbuffers;

  atHashMap<int64_t, atString> m_textures;
  atHashMap<int64_t, atString> m_samplers;

  // [Key] == the semantic of the input
  atHashMap<atString, atTypeDesc> m_inputs;

  static atTypeDesc HLSLTypeDesc(const atString &type);

  const atShaderParser& operator=(const atShaderParser &rhs);

protected:
  bool ReadFile();
  
  atFilename m_fn;
  atString m_src;
};

class atShaderInfo
{
};

#endif // atShaderParser_h__
