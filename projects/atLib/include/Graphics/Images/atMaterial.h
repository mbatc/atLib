
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

#ifndef _atMaterial_h__
#define _atMaterial_h__

#include "atFilename.h"
#include "atMath.h"

enum atMaterialIlluminationFlags
{
  atMIM_Colour = 1,
  atMIM_Ambient = 1 << 2,
  atMIM_Highlights = 1 << 3,
  atMIM_Reflections = 1 << 4,
  atMIM_RayTrace = 1 << 5,
  atMIM_Transparency = 1 << 6,
  atMIM_ReflectionFresnel = 1 << 7,
  atMIM_ReflectionRayTrace = 1 << 8,
  atMIM_Refraction = 1 << 9
};

class atMaterial
{
public:
  atMaterial();
  
  atString m_name;

  atVec4D m_cDiffuse = atVec4D::one();
  atVec4D m_cSpecular = atVec4D::one();
  atVec4D m_cAmbient = atVec4D::zero();

  double m_alpha = 1.0;
  double m_specularPower = 32.0;

  atVector<atFilename> m_tAmbient;
  atVector<atFilename> m_tDiffuse;
  atVector<atFilename> m_tAlpha;
  atVector<atFilename> m_tSpecular;
  atVector<atFilename> m_tSpecularHigh;
  atVector<atFilename> m_tBump;
  atVector<atFilename> m_tDisplacement;

  int64_t m_illumFlags;
};

int64_t atStreamRead(atReadStream *pStream, atMaterial *pData, const int64_t count);
int64_t atStreamWrite(atWriteStream *pStream, const atMaterial *pData, const int64_t count);

#endif