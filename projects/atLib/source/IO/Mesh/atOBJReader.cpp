
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

#include "atOBJParser.h"
#include "atFile.h"
#include "atScan.h"
#include "atMTLParser.h"
#include "atHashMap.h"

static const int64_t s_bufSize = 32;
static char s_buffer[s_bufSize] = { 0};

static atOBJKeyword _ScanKeyword(char *pSrc, int64_t srcLen, int64_t *pLen = nullptr)
{
  if (!atScan::String(s_buffer, s_bufSize, pSrc, srcLen, pLen))
    return atOBJNone;
  if (atString::compare(s_buffer, "v", atSCO_None)) return atOBJVertex;
  else if (atString::compare(s_buffer, "vn", atSCO_None)) return atOBJNormal;
  else if (atString::compare(s_buffer, "vt", atSCO_None)) return atOBJTexCoord;
  else if (atString::compare(s_buffer, "mtllib", atSCO_None)) return atOBJMatLib;
  else if (atString::compare(s_buffer, "usemtl", atSCO_None)) return atOBJMatRef;
  else if (atString::compare(s_buffer, "s", atSCO_None)) return atOBJSmoothShading;
  else if (atString::compare(s_buffer, "f", atSCO_None)) return atOBJFace;
  else if (atString::compare(s_buffer, "l", atSCO_None)) return atOBJLine;
  else if (atString::compare(s_buffer, "#", atSCO_None)) return atOBJComment;
  return atOBJNone;
}

static atOBJKeyword _ScanKeyword(char **ppSrc, int64_t srcLen, int64_t *pLen = nullptr)
{
  int64_t len = 0;
  atOBJKeyword ret = _ScanKeyword(*ppSrc, srcLen, &len);
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

static void _ParseFace(char **ppSrc, const int64_t srcLen, atVector<atMesh::Triangle> *pTris, const int64_t matID)
{
  int64_t start = atString::_find_first_not(*ppSrc, srcLen, atString::Whitespace());
  int64_t pos = start;
  int64_t newLine = atString::_find_first_of(*ppSrc, srcLen, '\n');
  int64_t vertCount = 0;
 
  // Count Verts
  while (pos < newLine)
  {
    vertCount++;
    pos = atString::_find_first_not(*ppSrc, srcLen, atString::Whitespace(), pos);
    pos = atString::_find_first_of(*ppSrc, srcLen, atString::Whitespace(), pos);
    if (pos < 0)
      break;
  }

  int64_t faceCount = vertCount - 2;
  if (faceCount < 1)
    return;

  // Scan face definition and triangulate
  pos = start;
  atMesh::Vertex firstVert;
  atMesh::Vertex lastVert;
  firstVert.position = atOBJInvalidIndex;
  
  for (int64_t i = 0; i < faceCount; ++i)
  {
    pTris->push_back(atMesh::Triangle());
    atMesh::Triangle &tri = pTris->at(pTris->size() - 1);
    tri.mat = matID;

    bool readStart = firstVert.position == atOBJInvalidIndex;
    if (!readStart)
    {
      tri.verts[0] = firstVert;
      tri.verts[1] = lastVert;
    }

    for (int64_t v = readStart ? 0 : 2; v < 3; ++v)
    {
      int64_t len = 0;
      int64_t slashIndex = 0;
      while (atString::_find_first_not(*ppSrc, srcLen, atString::Whitespace(), pos) == pos && slashIndex <= 2)
      {
        len = 0;
        int64_t val = atScan::Int(*ppSrc + pos, &len, srcLen);
        if (len > 0)
        {
          switch (slashIndex)
          {
          case 0: tri.verts[v].position = len > 0 ? val : AT_INVALID_INDEX; break;
          case 1: tri.verts[v].texCoord = len > 0 ? val : AT_INVALID_INDEX; break;
          case 2: tri.verts[v].normal = len > 0 ? val : AT_INVALID_INDEX; break;
          }
          pos += len;
        }
        pos += (*ppSrc)[pos] == '/';
        slashIndex++;
      }

      if (i == 0)
        firstVert = tri.verts[0];

      lastVert = tri.verts[2];
      pos = atString::_find_first_not(*ppSrc, srcLen, atString::Whitespace(), pos);
    }
  }
}

bool atOBJReader::Read(const atFilename &file, atMesh *pMesh)
{
  atFile reader;
  if (!reader.Open(file, atFM_Read))
    return false;

  atVector<uint8_t> data(reader.Info().Size(), 0);
  reader.Read(data.data(), data.size());
  reader.Close();

  char *pSrc = (char*)data.data();

  // Reserve memory based on file size to reduce allocations
  pMesh->m_normals.reserve(data.size() / 60);
  pMesh->m_triangles.reserve(data.size() / 180);
  pMesh->m_positions.reserve(data.size() / 60);
  pMesh->m_texCoords.reserve(data.size() / 60);

  atString mtlFile = "";
  atString curMat = "";
  atHashMap<atString, int64_t> matNames;
  int64_t matID = 0;

  while ((uint8_t*)pSrc < data.end())
  {
    switch (_ScanKeyword(&pSrc, data.end() - (uint8_t*)pSrc))
    {
    case atOBJComment: pSrc += atString::_find_end(pSrc, data.end() - (uint8_t*)pSrc, "\n"); break;
    case atOBJFace: _ParseFace(&pSrc, data.end() - (uint8_t*)pSrc, &pMesh->m_triangles, matNames[curMat]); break;
    case atOBJVertex: pMesh->m_positions.push_back(atOBJReader::ParseVector<atVec3D>(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atOBJNormal: pMesh->m_normals.push_back(atOBJReader::ParseVector<atVec3D>(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atOBJTexCoord: pMesh->m_texCoords.push_back(atOBJReader::ParseVector<atVec3D>(&pSrc, data.end() - (uint8_t*)pSrc).xy()); break;
    case atOBJLine: pSrc += atString::_find_first_of(pSrc, data.end() - (uint8_t*)pSrc, "\n\r"); break;
    case atOBJMatLib: mtlFile = _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc); break;
    case atOBJMatRef: 
      curMat = _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc);
      matNames.TryAdd(curMat, matNames.Size());
      break;
    case atOBJNone: ++pSrc;
    }
  }

  for(atMesh::Triangle &tri : pMesh->m_triangles)
    for (int64_t i = 0; i < 3; ++i)
    {
      tri.verts[i].position = tri.verts[i].position > 0 ? tri.verts[i].position - 1 : pMesh->m_positions.size() + tri.verts[i].position;
      tri.verts[i].texCoord = tri.verts[i].texCoord > 0 ? tri.verts[i].texCoord - 1 : pMesh->m_texCoords.size() + tri.verts[i].texCoord;
      tri.verts[i].normal = tri.verts[i].normal > 0 ? tri.verts[i].normal - 1 : pMesh->m_normals.size() + tri.verts[i].normal;
      tri.verts[i].color = tri.verts[i].color > 0 ? tri.verts[i].color - 1 : pMesh->m_colors.size() + tri.verts[i].color;
    }

  pMesh->m_normals.shrink_to_fit();
  pMesh->m_triangles.shrink_to_fit();
  pMesh->m_positions.shrink_to_fit();
  pMesh->m_texCoords.shrink_to_fit();

  pMesh->m_sourceFile = file.Path();
  pMesh->m_resourceDir = file.Directory();
  atMTLReader::Read(pMesh->TryDiscoverFile(mtlFile, ""), pMesh, matNames);
  return true;
}