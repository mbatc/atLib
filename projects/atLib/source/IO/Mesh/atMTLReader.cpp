
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
  if (atString::compare(s_buffer, "Ka", atSCO_None)) return atMTLAmbientColour;
  else if (atString::compare(s_buffer, "Kd", atSCO_None)) return atMTLDiffuseColour;
  else if (atString::compare(s_buffer, "Ks", atSCO_None)) return atMTLSpecularColour;
  else if (atString::compare(s_buffer, "Ns", atSCO_None)) return atMTLSpecularPower;
  else if (atString::compare(s_buffer, "Tr", atSCO_None)) return atMTLAlphaInv;
  else if (atString::compare(s_buffer, "d", atSCO_None)) return atMTLAlpha;
  else if (atString::compare(s_buffer, "illum", atSCO_None)) return atMTLIllumModel;
  else if (atString::compare(s_buffer, "map_Ka", atSCO_None)) return atMTLAmbientMap;
  else if (atString::compare(s_buffer, "map_Kd", atSCO_None)) return atMTLDiffuseMap;
  else if (atString::compare(s_buffer, "map_Ks", atSCO_None)) return atMTLSpecularColourMap;
  else if (atString::compare(s_buffer, "map_Ns", atSCO_None)) return atMTLSpecularHighlightMap;
  else if (atString::compare(s_buffer, "map_d", atSCO_None)) return atMTLAlphaMap;
  else if (atString::compare(s_buffer, "map_bump", atSCO_None)) return atMTLBumpMap;
  else if (atString::compare(s_buffer, "bump", atSCO_None)) return atMTLBumpMap;
  else if (atString::compare(s_buffer, "disp", atSCO_None)) return atMTLDisplacementMap;
  else if (atString::compare(s_buffer, "newmtl", atSCO_None)) return atMTLNew;
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
  int64_t start = atString::_find_first_not(*ppSrc, atString::Whitespace());
  int64_t end = atString::_find_first_of(*ppSrc, "\r\n", start);
  *ppSrc += end;
  return atString(*ppSrc - end + start, *ppSrc);
}

static atMaterial* _GetMaterial(atMesh *pMesh, const atString &name, bool addMat)
{
  for (atMaterial &mat : pMesh->m_materials)
    if (mat.GetName() == name)
      return &mat;
  if (addMat)
  {
    pMesh->m_materials.push_back(atMaterial());
    (pMesh->m_materials.end() - 1)->SetName(name);
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
    pMesh->m_materials[kvp.m_val].SetName(kvp.m_key);

  atMaterial *pMat = nullptr;
  while ((uint8_t*)pSrc < data.end())
  {
    if (!pMat) pSrc += atString::_find(pSrc, "newmtl");
    switch (_ScanKeyword(&pSrc, data.end() - (uint8_t*)pSrc))
    {
    case atMTLNew: pMat = _GetMaterial(pMesh, _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc), loadAll); break;
    case atMTLAmbientColour: pMat->SetColour(atMP_Ambient, { atOBJReader::ParseVector<atVec3D>(&pSrc, data.end() - (uint8_t*)pSrc), 1.0 }); break;
    case atMTLDiffuseColour: pMat->SetColour(atMP_Diffuse, { atOBJReader::ParseVector<atVec3D>(&pSrc, data.end() - (uint8_t*)pSrc), 1.0 }); break;
    case atMTLSpecularColour: pMat->SetColour(atMP_Specular, { atOBJReader::ParseVector<atVec3D>(&pSrc, data.end() - (uint8_t*)pSrc), 1.0 }); break;
    case atMTLSpecularPower: pMat->SetValue(atMP_SpecularPower, atScan::Float((const char**)&pSrc)); break;
    case atMTLAlpha: pMat->SetValue(atMP_Alpha, atScan::Float((const char**)&pSrc)); break;
    case atMTLAlphaInv: pMat->SetValue(atMP_Alpha, 1.0 - atScan::Float((const char**)&pSrc)); break;
    case atMTLAmbientMap: pMat->SetTexture(atMP_Ambient, _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atMTLDiffuseMap: pMat->SetTexture(atMP_Diffuse, _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atMTLSpecularColourMap: pMat->SetTexture(atMP_Specular, _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atMTLSpecularHighlightMap:pMat->SetTexture(atMP_SpecularHighlight, _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atMTLAlphaMap: pMat->SetTexture(atMP_Alpha, _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atMTLBumpMap: pMat->SetTexture(atMP_Bump, _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atMTLDisplacementMap: pMat->SetTexture(atMP_Displacement, _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atMTLIllumModel: break;
    case atMTLNone: ++pSrc; break;
    }
  }

  pMesh->m_resourceDir = file.Directory();
  return true;
}