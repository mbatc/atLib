
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

#include "atMaterial.h"
#include "atFile.h"

atMaterial::atMaterial()
  : m_cDiffuse(atVec4F(1.0f, 1.0f, 1.0f, 1.0f))
  , m_cAmbient(atVec4F(0.0f, 0.0f, 0.0f, 0.0f))
  , m_cSpecular(atVec4F(1.0f, 1.0f, 1.0f, 1.0f))
{}

int64_t atStreamRead(atReadStream *pStream, atMaterial *pData, const int64_t count)
{
  int64_t size = 0;
  for (atMaterial &mat : atIterate(pData, count))
  {
    size += atStreamRead(pStream, &mat.m_name, 1);
    size += atStreamRead(pStream, &mat.m_alpha, 1);
    size += atStreamRead(pStream, &mat.m_specularPower, 1);
    size += atStreamRead(pStream, &mat.m_cAmbient, 1);
    size += atStreamRead(pStream, &mat.m_cDiffuse, 1);
    size += atStreamRead(pStream, &mat.m_cSpecular, 1);
    size += atStreamRead(pStream, &mat.m_tAmbient, 1);
    size += atStreamRead(pStream, &mat.m_tDiffuse, 1);
    size += atStreamRead(pStream, &mat.m_tSpecular, 1);
    size += atStreamRead(pStream, &mat.m_tSpecularHigh, 1);
    size += atStreamRead(pStream, &mat.m_tDisplacement, 1);
    size += atStreamRead(pStream, &mat.m_tBump, 1);
    size += atStreamRead(pStream, &mat.m_tAlpha, 1);
  }

  return size;
}

int64_t atStreamWrite(atWriteStream *pStream, const atMaterial *pData, const int64_t count)
{
  int64_t size = 0;
  for (const atMaterial &mat : atIterate(pData, count))
  {
    size += atStreamWrite(pStream, &mat.m_name, 1);
    size += atStreamWrite(pStream, &mat.m_alpha, 1);
    size += atStreamWrite(pStream, &mat.m_specularPower, 1);
    size += atStreamWrite(pStream, &mat.m_cAmbient, 1);
    size += atStreamWrite(pStream, &mat.m_cDiffuse, 1);
    size += atStreamWrite(pStream, &mat.m_cSpecular, 1);
    size += atStreamWrite(pStream, &mat.m_tAmbient, 1);
    size += atStreamWrite(pStream, &mat.m_tDiffuse, 1);
    size += atStreamWrite(pStream, &mat.m_tSpecular, 1);
    size += atStreamWrite(pStream, &mat.m_tSpecularHigh, 1);
    size += atStreamWrite(pStream, &mat.m_tDisplacement, 1);
    size += atStreamWrite(pStream, &mat.m_tBump, 1);
    size += atStreamWrite(pStream, &mat.m_tAlpha, 1);
  }
  return size;
}
