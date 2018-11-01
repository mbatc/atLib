
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

#include "atMesh.h"
#include "atHashMap.h"
#include "atFile.h"
#include "atMeshParser.h"

bool atMesh::Import(const atString &path) { return atMeshReader::Read(path, this); }
bool atMesh::Export(const atString &path) { return atMeshWriter::Write(path, *this); }

atMesh::atMesh() {}
atMesh::atMesh(atMesh &&move) { *this = move; }
atMesh::atMesh(const atMesh &copy) { *this = copy; }

bool atMesh::MakeValid()
{
  for (Triangle &tri : m_triangles)
  {
    for (int64_t v = 0; v < 3; ++v)
    {
      if (tri.verts[v].color == AT_INVALID_INDEX)
      {
        if (m_default.color == AT_INVALID_INDEX)
        {
          m_default.color = m_colors.size();
          m_colors.push_back(atVec4F64(1.0, 1.0, 1.0, 1.0));
        }
        tri.verts[v].color = m_default.color;
      }

      if (tri.verts[v].position == AT_INVALID_INDEX)
      {
        if (m_default.position == AT_INVALID_INDEX)
        {
          m_default.position = m_positions.size();
          m_positions.push_back(atVec3F64(0.0, 0.0, 0.0));
        }
        tri.verts[v].position = m_default.position;
      }

      if (tri.verts[v].texCoord == AT_INVALID_INDEX)
      {
        if (m_default.texCoord == AT_INVALID_INDEX)
        {
          m_default.texCoord = m_texCoords.size();
          m_texCoords.push_back(atVec2F64(0.0, 0.0));
        }
        tri.verts[v].texCoord = m_default.texCoord;
      }
    }

    // calculate missing normals
    for (int64_t v = 0; v < 3; ++v)
      if (tri.verts[v].normal == AT_INVALID_INDEX)
      {
        tri.verts[v].normal = m_normals.size();
        m_normals.push_back(atVectorMath::Cross(m_positions[tri.verts[v].position] - m_positions[tri.verts[(v - 1) % 3].position],
          m_positions[tri.verts[v].position] - m_positions[tri.verts[(v + 1) % 3].position]));
      }

    if (tri.mat == AT_INVALID_INDEX)
      if (m_defaultMat == AT_INVALID_INDEX)
      {
        m_defaultMat = m_materials.size();
        m_materials.push_back(atMaterial());
      }
  }

  GenTangents();

  // If there are no triangles this mesh cannot be made valid
  return m_triangles.size() != 0;
}

void atMesh::SpatialTransform(const atMat4D &transform)
{
  PositionTransform(transform);
  NormalTransform(transform);
}

void atMesh::PositionTransform(const atMat4D &transform)
{
  for (atVec3F64 &pos : m_positions)
    pos = transform * pos;
}

void atMesh::NormalTransform(const atMat4D &transform)
{
  atMat4D nMat = transform.Inverse().Transpose();
  for (atVec3F64 &norm : m_normals)
    norm = transform * norm;
}

void atMesh::DiscoverTextures(const atString &initialPath)
{
  for (atMaterial &mat : m_materials)
  {
    for (atFilename &path : mat.m_tDiffuse)
      path = TryDiscoverFile(path.Path(), initialPath);
    for (atFilename &path : mat.m_tAmbient)
      path = TryDiscoverFile(path.Path(), initialPath);
    for (atFilename &path : mat.m_tSpecular)
      path = TryDiscoverFile(path.Path(), initialPath);
    for (atFilename &path : mat.m_tAlpha)
      path = TryDiscoverFile(path.Path(), initialPath);
    for (atFilename &path : mat.m_tDisplacement)
      path = TryDiscoverFile(path.Path(), initialPath);
    for (atFilename &path : mat.m_tSpecularHigh)
      path = TryDiscoverFile(path.Path(), initialPath);
  }
}

void atMesh::GenTangents()
{
  m_tangents.resize(m_positions.size());
  m_binormals.resize(m_positions.size());
  memset(m_tangents.data(), 0, m_tangents.size() * sizeof(atVec3F64));
  memset(m_binormals.data(), 0, m_binormals.size() * sizeof(atVec3F64));

  for (Triangle &tri : m_triangles)
  {
    const atVec3F64 &v1 = m_positions[tri.verts[0].position];
    const atVec3F64 &v2 = m_positions[tri.verts[1].position];
    const atVec3F64 &v3 = m_positions[tri.verts[2].position];
    const atVec2F64 &t1 = m_texCoords[tri.verts[0].texCoord];
    const atVec2F64 &t2 = m_texCoords[tri.verts[1].texCoord];
    const atVec2F64 &t3 = m_texCoords[tri.verts[2].texCoord];

    const atVec3F64 AB = v2 - v1;
    const atVec3F64 AC = v3 - v1;
    const atVec2F64 tAB = t2 - t1;
    const atVec2F64 tAC = t3 - t1;

    double r = 1.0 / (tAB.x * tAC.y - tAC.x * tAB.y);
    atVec3F64 uDir = atVec3F64{ (tAC.y * AB.x - tAB.y * AC.x), (tAC.y * AB.y - tAB.y * AC.y), (tAC.y * AB.z - tAB.y * AC.z) } * r;
    atVec3F64 vDir = atVec3F64{ (tAB.x * AC.x - tAC.x * AB.x), (tAB.x * AC.y - tAC.x * AB.y), (tAB.x * AC.z - tAC.x * AB.z) } * r;

    for (int64_t i = 0; i < 3; ++i)
    {
      tri.verts[i].tangent = tri.verts[i].position;
      tri.verts[i].bitanget = tri.verts[i].position;
      m_tangents[tri.verts[i].position] += uDir;
      m_binormals[tri.verts[i].position] += vDir;
    }
  }

  for (int64_t i = 0; i < m_positions.size(); ++i)
  {
    m_tangents[i] = m_tangents[i].Normalize();
    m_binormals[i] = m_binormals[i].Normalize();
  }
}

void atMesh::RegenNormals()
{
  m_normals.clear();
  atHashMap<atVec3F64, int64_t> normMap;
  for (Triangle &tri : m_triangles)
  {
    atVec3F64 n = (m_positions[tri.verts[0].position] - m_positions[tri.verts[1].position]).Cross(m_positions[tri.verts[2].position] - m_positions[tri.verts[1].position]);
    int64_t normIndex = m_normals.size();
    if (!normMap.TryAdd(n, normIndex))
    {
      normIndex = normMap[n];
      m_normals.push_back(n);
    }
    for (int64_t i = 0; i < 3; ++i)
      tri.verts[i].normal = normIndex;
  }
}

void atMesh::GenSmoothNormals(const double threshold, const bool regenNormals)
{
  if (regenNormals)
    RegenNormals();
}

atString atMesh::TryDiscoverFile(const atString &file, const atString &initialDir)
{
  // Check if the path is already valid
  if (atFile::Exists(file))
    return file;

  // Check if the path is relative to the initialDir
  atString path;
  if (initialDir.length() != 0)
  {
    path = initialDir + "/" + file;
    if (atFile::Exists(path))
      return path;
  }

  // Check if the path is relative to the resource directory
  path = m_resourceDir + "/" + file;
  if (atFile::Exists(path))
    return path;

  // Check if the path is relative to the source file
  path = atFilename(m_sourceFile).Directory() + "/" + file;
  if (atFile::Exists(path))
    return path;

  // Check if the texture is next to the source file
  path = atFilename(m_sourceFile).Directory() + "/" + atFilename(file).Name();
  if (atFile::Exists(path))
    return path;

  // Check if the texture is in a sub folder with the same name as the source file
  path = atFilename(m_sourceFile).Directory() + "/" + atFilename(m_sourceFile).Name(false) + atFilename(file).Name();
  if (atFile::Exists(path))
    return path;

  return file;
}

atVector<atTriangle<double>> atMesh::GetTriangles()
{
  atVector<atTriangle<double>> ret;
  for (Triangle &tri : m_triangles)
    ret.push_back(atTriangle<double>(
      m_positions[tri.verts[0].position], 
      m_positions[tri.verts[1].position], 
      m_positions[tri.verts[2].position]));
  return ret;
}

const atMesh& atMesh::operator=(const atMesh &rhs)
{
  m_sourceFile = rhs.m_sourceFile;
  m_resourceDir = rhs.m_resourceDir;
  m_positions = rhs.m_positions;
  m_normals = rhs.m_normals;
  m_colors = rhs.m_colors;
  m_texCoords = rhs.m_texCoords;
  m_tangents = rhs.m_tangents;
  m_binormals = rhs.m_binormals;
  m_materials = rhs.m_materials;
  m_default = rhs.m_default;
  m_defaultMat = rhs.m_defaultMat;
  m_triangles = rhs.m_triangles;
  return *this;
}

const atMesh& atMesh::operator=(atMesh && rhs)
{
  m_sourceFile = std::move(rhs.m_sourceFile);
  m_resourceDir = std::move(rhs.m_resourceDir);
  m_positions = std::move(rhs.m_positions);
  m_normals = std::move(rhs.m_normals);
  m_colors = std::move(rhs.m_colors);
  m_texCoords = std::move(rhs.m_texCoords);
  m_tangents = std::move(rhs.m_tangents);
  m_binormals = std::move(rhs.m_binormals);
  m_materials = std::move(rhs.m_materials);
  m_default = std::move(rhs.m_default);
  m_defaultMat = std::move(rhs.m_defaultMat);
  m_triangles = std::move(rhs.m_triangles);
  rhs.m_default = Vertex();
  rhs.m_defaultMat = AT_INVALID_INDEX;
  return *this;
}

int64_t atStreamRead(atReadStream *pStream, atMesh::Face *pData, const int64_t count)
{
  int64_t size = 0;
  for (atMesh::Face &face : atIterate(pData, count))
  {
    size += pStream->Read(&face.mat);
    size += pStream->Read(&face.verts);
  }
  return size;
}

int64_t atStreamWrite(atWriteStream *pStream, const atMesh::Face *pData, const int64_t count)
{
  int64_t size = 0;
  for (const atMesh::Face &face : atIterate(pData, count))
  {
    pStream->Write(face.mat);
    size += pStream->Write(face.verts);
  }
  return size;
}

int64_t atStreamRead(atReadStream *pStream, atMesh *pData, const int64_t count)
{
  int64_t size = 0;
  for (atMesh &mesh : atIterate(pData, count))
  {
    size += pStream->Read(&mesh.m_sourceFile);
    size += pStream->Read(&mesh.m_resourceDir);
    size += pStream->Read(&mesh.m_default);

    size += pStream->Read(&mesh.m_triangles);
    size += pStream->Read(&mesh.m_positions);
    size += pStream->Read(&mesh.m_normals);
    size += pStream->Read(&mesh.m_colors);
    size += pStream->Read(&mesh.m_tangents);
    size += pStream->Read(&mesh.m_binormals);
    size += pStream->Read(&mesh.m_texCoords);

    size += pStream->Read(&mesh.m_materials);
  }
  return size;
}

int64_t atStreamWrite(atWriteStream *pStream, const atMesh *pData, const int64_t count)
{
  int64_t size = 0;
  for (const atMesh &mesh : atIterate(pData, count))
  {
    size += pStream->Write(mesh.m_sourceFile);
    size += pStream->Write(mesh.m_resourceDir);
    size += pStream->Write(mesh.m_default);

    size += pStream->Write(mesh.m_triangles);
    size += pStream->Write(mesh.m_positions);
    size += pStream->Write(mesh.m_normals);
    size += pStream->Write(mesh.m_colors);
    size += pStream->Write(mesh.m_tangents);
    size += pStream->Write(mesh.m_binormals);
    size += pStream->Write(mesh.m_texCoords);

    size += pStream->Write(mesh.m_materials);
  }
  return size;
}

void atMesh::FlipNormals() { for (atVec3F64 &norm : m_normals) norm = -norm; }
void atMesh::FlipTextures(const bool u, const bool v) { if (!(u || v)) return; for (atVec2F64 &texCoord : m_texCoords) texCoord = { u ? 1.0 - texCoord.x : texCoord.x, v ? 1.0 - texCoord.y : texCoord.y }; }
