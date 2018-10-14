#include "atOBJParser.h"
#include "atFile.h"
#include "atScan.h"

static const int64_t s_bufSize = 32;
static char s_buffer[s_bufSize] = { 0};

static atOBJKeyword _ScanKeyword(char *pSrc, int64_t *pLen = nullptr)
{
  if (!atScan::String(s_buffer, s_bufSize, pSrc, pLen))
    return atOBJNone;
  if (atString::compare(s_buffer, "v")) return atOBJVertex;
  else if (atString::compare(s_buffer, "vn")) return atOBJNormal;
  else if (atString::compare(s_buffer, "vt")) return atOBJTexCoord;
  else if (atString::compare(s_buffer, "mtllib")) return atOBJMatLib;
  else if (atString::compare(s_buffer, "usemtl")) return atOBJMatRef;
  else if (atString::compare(s_buffer, "f")) return atOBJFace;
  else if (atString::compare(s_buffer, "l")) return atOBJLine;
  return atOBJNone;
}

static atOBJKeyword _ScanKeyword(char **ppSrc, int64_t *pLen = nullptr)
{
  int64_t len = 0;
  atOBJKeyword ret = _ScanKeyword(*ppSrc, &len);
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
  int64_t pos = atString::_find_first_not(*ppSrc, srcLen, atString::Whitespace());
  int64_t newLine = atString::_find_first_of(*ppSrc, srcLen, '\n');
  int64_t vertCount = 0;
  
  while (pos < newLine)
  {
    vertCount++;
    pos = atString::_find_first_not(*ppSrc, srcLen, atString::Whitespace(), pos);
    pos = atString::_find_first_of(*ppSrc, srcLen, atString::Whitespace(), pos);
  }

  int64_t faceCount = vertCount - 2;
  if (faceCount < 1)
    return;

  atMesh::Vertex startVert;
  int64_t texIndex = atOBJInvalidIndex;
  int64_t posIndex = atOBJInvalidIndex;
  int64_t normIndex = atOBJInvalidIndex;
  
  for (int64_t i = 0; i < faceCount; ++i)
  {
    pTris->push_back(atMesh::Triangle());
    atMesh::Triangle &tri = pTris->at(pTris->size() - 1);
    bool readStart = startVert.position == atOBJInvalidIndex;
    if (!readStart) tri.vert[0] = startVert;
    for (int64_t i = readStart ? 0 : 1; i < 3; ++i)
    {


      if (i == 0)
        startVert = tri.vert[0];
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
  char *pSrc = (char*)data.data();

  // Reserve memory based on file size to reduce allocations
  pMesh->m_normals.reserve(data.size() / 48);
  pMesh->m_triangles.reserve(data.size() / 48);
  pMesh->m_positions.reserve(data.size() / 48);
  pMesh->m_texCoords.reserve(data.size() / 32);

  atString mtlFile = "";
  atString matName = "";
  int64_t matID = 0;

  while ((uint8_t*)pSrc < data.end())
  {
    switch (_ScanKeyword(&pSrc))
    {
    case atOBJFace: _ParseFace(&pSrc, data.end() - (uint8_t*)pSrc, &pMesh->m_triangles, matID); break;
    case atOBJVertex: pMesh->m_positions.push_back(atOBJReader::ParseVector<atVec3F64>(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atOBJNormal: pMesh->m_positions.push_back(atOBJReader::ParseVector<atVec3F64>(&pSrc, data.end() - (uint8_t*)pSrc)); break;
    case atOBJTexCoord: pMesh->m_texCoords.push_back(atOBJReader::ParseVector<atVec3F64>(&pSrc, data.end() - (uint8_t*)pSrc).xy()); break;
    case atOBJLine: pSrc += atString::_find_first_of(pSrc, data.end() - (uint8_t*)pSrc, "\n\r"); break;
    case atOBJMatLib: mtlFile = _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc); break;
    case atOBJMatRef: matName = _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc); break;
    }
  }

  return true;
}