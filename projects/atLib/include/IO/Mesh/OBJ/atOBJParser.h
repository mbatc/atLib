
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

#ifndef atOBJParser_h__
#define atOBJParser_h__

#include "atMesh.h"

enum atOBJKeyword
{
  atOBJObject, // unsupported
  atOBJGroup, // unsupported
  atOBJSmoothShading, // unsupported
  atOBJVertex,
  atOBJNormal,
  atOBJTexCoord,
  atOBJLine,
  atOBJFace,
  atOBJMatLib,
  atOBJMatRef,
  atOBJComment,
  atOBJNone
};

#define atOBJInvalidIndex INT64_MAX

class atOBJReader
{
public:
  atOBJReader() = delete;
  static bool Read(const atFilename &file, atMesh *pMesh);
  
  // This probably belongs in atScan as it is a very generic string parsing function
  template<typename T> static T ParseVector(char **ppSrc, const int64_t srcLen, int64_t *pLen = nullptr);
};

class atOBJWriter
{
public:
  atOBJWriter() = delete;
  static bool Write(const atFilename &file, const atMesh &mesh);
};

#include "atOBJReader.inl"
#endif // atOBJParser_h__
