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

static atString _ReadLine(char **ppSrc, int64_t len)
{
  int64_t start = atString::_find_first_not(*ppSrc, len, atString::Whitespace());
  int64_t end = atString::_find_first_of(*ppSrc, len, "\r\n", start);
  *ppSrc += end;
  return atString(*ppSrc - end + start, *ppSrc);
}

static atOBJKeyword _ScanKeyword(char **ppSrc, int64_t *pLen = nullptr)
{
  int64_t len = 0;
  atOBJKeyword ret = _ScanKeyword(*ppSrc, &len);
  *ppSrc += len;
  if (pLen) *pLen = len;
  return ret;
}

static atVec3F64 _ParseVec3(char **ppSrc, int64_t *pLen = nullptr)
{
  return atVec3F64();
}

static atVec2F64 _ParseVec2(char **ppSrc, int64_t *pLen = nullptr)
{
  return atVec2F64();
}

// Parse Face
static atMesh::Triangle _ParseFace(char **ppSrc, atVector<atMesh::Triangle> *pTris, const int64_t matID)
{
  pTris->push_back(atMesh::Triangle());
  atMesh::Triangle tri
  return atMesh::Triangle();
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
    case atOBJFace: _ParseFace(&pSrc, &pMesh->m_triangles, matID); break;
    case atOBJVertex: pMesh->m_positions.push_back(_ParseVec3(&pSrc)); break;
    case atOBJNormal: pMesh->m_positions.push_back(_ParseVec3(&pSrc)); break;
    case atOBJTexCoord: pMesh->m_texCoords.push_back(_ParseVec3(&pSrc).xy()); break;
    case atOBJLine: pSrc += atString::_find_first_of(pSrc, data.end() - (uint8_t*)pSrc, "\n\r"); break;
    case atOBJMatLib: mtlFile = _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc); break;
    case atOBJMatRef: matName = _ReadLine(&pSrc, data.end() - (uint8_t*)pSrc); break;
    }
  }

  return true;
}