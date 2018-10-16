
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

#ifndef atMTLParser_h__
#define atMTLParser_h__

#include "atMesh.h"
#include "atHashMap.h"

enum atMTLKeyword
{
  atMTLNone,
  atMTLNew,
  atMTLAmbientColour,
  atMTLDiffuseColour,
  atMTLSpecularColour,
  atMTLSpecularPower,
  atMTLAlpha,
  atMTLAlphaInv,
  atMTLAmbientMap,
  atMTLDiffuseMap,
  atMTLSpecularColourMap,
  atMTLSpecularHighlightMap,
  atMTLAlphaMap,
  atMTLBumpMap,
  atMTLDisplacementMap,
  atMTLIllumModel
};

class atMTLReader
{
public:
  atMTLReader() = delete;
  static bool Read(const atFilename &file, atMesh *pMesh, const atHashMap<atString, int64_t> &materials = atHashMap<atString, int64_t>());
};

class atMTLWriter
{
public:
  atMTLWriter() = delete;
  static bool Write(const atFilename &file, const atMesh &mesh);
};

#endif // atMTLParser_h__
