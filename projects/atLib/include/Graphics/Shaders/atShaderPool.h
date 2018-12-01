
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

#ifndef _atShaderPool_h__
#define _atShaderPool_h__

#include "atString.h"
#include "atRenderable.h"

class atShaderPool
{
public:
  static int64_t GetShader(const atString &filename);
  
  static void ReleaseShader(const int64_t id);
  static void ReloadShaders();
  
  static int64_t ShaderRound();
  static int64_t BindShader(const int64_t id);
  static int64_t BindInputLayout(const int64_t id);
  static int64_t GetInputLayout(const int64_t id, const atVector<VertexData> &desc);

  // Set a variable in the currently bound shader
  static bool SetVariable(const int64_t shader, const atString &name, void *pData, int64_t len);
  static bool SetVariable(const int64_t shader, const int64_t loc, void *pData, int64_t len);
  static int64_t GetVariableLoc(const int64_t shader, const atString &name);

  static bool IsValid(const int64_t id);
};

#endif