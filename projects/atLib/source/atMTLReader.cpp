
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

#include "atMTLParser.h"
#include "atOBJParser.h"
#include "atFile.h"
#include "atScan.h"

static const int64_t s_bufSize = 64;
static char s_buffer[s_bufSize] = { 0 };

static atMTLKeyword _ScanKeyword(char *pSrc, int64_t srcLen, int64_t *pLen = nullptr)
{
  if (!atScan::String(s_buffer, s_bufSize, pSrc, srcLen, pLen))
    return atMTLNone;
  if (atString::compare(s_buffer, "Ka")) return atMTLAmbientColour;
  else if (atString::compare(s_buffer, "Kd")) return atMTLDiffuseColour;
  else if (atString::compare(s_buffer, "Ks")) return atMTLSpecularColour;
  else if (atString::compare(s_buffer, "Ns")) return atMTLSpecularPower;
  else if (atString::compare(s_buffer, "Tr")) return atMTLAlphaInv;
  else if (atString::compare(s_buffer, "d")) return atMTLAlpha;
  else if (atString::compare(s_buffer, "illum")) return atMTLIllumModel;
  else if (atString::compare(s_buffer, "map_Ka")) return atMTLAmbientMap;
  else if (atString::compare(s_buffer, "map_Kd")) return atMTLDiffuseMap;
  else if (atString::compare(s_buffer, "map_Ks")) return atMTLSpecularColourMap;
  else if (atString::compare(s_buffer, "map_Ns")) return atMTLSpecularHighlightMap;
  else if (atString::compare(s_buffer, "map_d")) return atMTLAlphaMap;
  else if (atString::compare(s_buffer, "map_bump")) return atMTLBumpMap;
  else if (atString::compare(s_buffer, "bump")) return atMTLBumpMap;
  else if (atString::compare(s_buffer, "disp")) return atMTLDisplacementMap;
  else if (atString::compare(s_buffer, "newmtl")) return atMTLNew;
  return atMTLNone;
}

static atMTLKeyword _ScanKeyword(char **ppSrc, int64_t srcLen, int64_t *pLen = nullptr)
{
  int64_t len = 0;
  atMTLKeyword ret = _ScanKeyword(*ppSrc, srcLen, &len);
  *ppSrc += len;
  if (pLen) *pLen = len;
  return ret;
}

static atString _ReadLine(char **ppSrc, int64_t len)
{
  int64_t start = atString::_find_first_not(*ppSrc, len, atString::Whitespace());
  int64_t end = atString::_find_first_of(*ppSrc, len, "\r\n", start);
  *ppSrc += end;
  return atString(*ppSrc - end + start, *ppSrc);
}

static atMaterial* _GetMaterial(atMesh *pMesh, const atString &name, bool addMat)
{
  for (atMaterial &mat : pMesh->m_materials)
    if (mat.m_name == name)
      return &mat;
  if (addMat)
  {
    pMesh->m_materials.push_back(atMaterial());
    (pMesh->m_materials.end() - 1)->m_name = name;
    return pMesh->m_materials.end() - 1;
  }
  return nullptr;
}

bool atMTLReader::Read(const atFilename &file, atMesh *pMesh, const atHashMap<atString, int64_t> &materials)
{
  bool loadAll = materials.Size() == 0;

  atFile reader;
  if (!reader.Open(file, atFM_Read))
    return false;

  atVector<uint8_t> data(reader.Info().Size(), 0);
  reader.Read(data.data(), data.size());
  reader.Close();

  char *pSrc = (char*)data.data();

  pMesh->m_materials.resize(materials.Size());
  for (auto &kvp : materials)
    pMesh->m_materials[kvp.m_val].m_name = kvp.m_key;

  atMaterial *pMat = nullptr;
  while ((uint8_t*)pSrc < data.end())
  {
    if (!pMat) pSrc += atString::_find(pSrc, data.end() - (uint8_t*)pSrc, "newmtl");
    switch (_ScanKeyword(&pSrc, data.end() - (uint8_t*)pSrc))
    {
    case atMTLNew: pMat = _GetMaterial(pMesh, _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc), loadAll); break;
    case atMTLAmbientColour: pMat->m_cAmbient = { atOBJReader::ParseVector<atVec3F64>(&pSrc, data.end() - (uint8_t*)pSrc), 1.0 }; break;
    case atMTLDiffuseColour: pMat->m_cDiffuse = { atOBJReader::ParseVector<atVec3F64>(&pSrc, data.end() - (uint8_t*)pSrc), 1.0 }; break;
    case atMTLSpecularColour: pMat->m_cAmbient = { atOBJReader::ParseVector<atVec3F64>(&pSrc, data.end() - (uint8_t*)pSrc), 1.0 }; break;
    case atMTLSpecularPower: pMat->m_specularPower = atScan::Float((const char**)&pSrc); break;
    case atMTLAlpha: pMat->m_alpha = atScan::Float((const char**)&pSrc); break;
    case atMTLAlphaInv: pMat->m_alpha = 1.0 - atScan::Float((const char**)&pSrc); break;
    case atMTLAmbientMap: pMat->m_tAmbient.push_back(_ReadLine(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atMTLDiffuseMap: pMat->m_tDiffuse.push_back(_ReadLine(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atMTLSpecularColourMap: pMat->m_tSpecular.push_back(_ReadLine(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atMTLSpecularHighlightMap: pMat->m_tSpecularHigh.push_back(_ReadLine(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atMTLAlphaMap: pMat->m_tAlpha.push_back(_ReadLine(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atMTLBumpMap: pMat->m_tBump.push_back(_ReadLine(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atMTLDisplacementMap: pMat->m_tDisplacement.push_back(_ReadLine(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atMTLIllumModel: break;
    case atMTLNone: ++pSrc; break;
    }
  }

  pMesh->m_resourceDir = file.Directory();
  return true;
}