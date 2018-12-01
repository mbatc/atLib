
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
    size += pStream->Read(&mat.m_name);
    size += pStream->Read(&mat.m_alpha);
    size += pStream->Read(&mat.m_specularPower);
    size += pStream->Read(&mat.m_cAmbient);
    size += pStream->Read(&mat.m_cDiffuse);
    size += pStream->Read(&mat.m_cSpecular);
    size += pStream->Read(&mat.m_tAmbient);
    size += pStream->Read(&mat.m_tDiffuse);
    size += pStream->Read(&mat.m_tSpecular);
    size += pStream->Read(&mat.m_tSpecularHigh);
    size += pStream->Read(&mat.m_tDisplacement);
    size += pStream->Read(&mat.m_tBump);
    size += pStream->Read(&mat.m_tAlpha);
  }
  return size;
}

int64_t atStreamWrite(atWriteStream *pStream, const atMaterial *pData, const int64_t count)
{
  int64_t size = 0;
  for (const atMaterial &mat : atIterate(pData, count))
  {
    size += pStream->Write(mat.m_name);
    size += pStream->Write(mat.m_alpha);
    size += pStream->Write(mat.m_specularPower);
    size += pStream->Write(mat.m_cAmbient);
    size += pStream->Write(mat.m_cDiffuse);
    size += pStream->Write(mat.m_cSpecular);
    size += pStream->Write(mat.m_tAmbient);
    size += pStream->Write(mat.m_tDiffuse);
    size += pStream->Write(mat.m_tSpecular);
    size += pStream->Write(mat.m_tSpecularHigh);
    size += pStream->Write(mat.m_tDisplacement);
    size += pStream->Write(mat.m_tBump);
    size += pStream->Write(mat.m_tAlpha);
  }
  return size;
}
