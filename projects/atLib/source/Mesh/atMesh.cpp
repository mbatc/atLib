
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

#include "atMesh.h"
#include "atHashMap.h"
#include "atFile.h"
#include "atMeshParser.h"

bool atMesh::Import(const atString &path) { return atMeshReader::Read(path, this); }
bool atMesh::Export(const atString &path) { return atMeshWriter::Write(path, *this); }

atMesh::atMesh() {}
atMesh::atMesh(atMesh &&move) { *this = move; }
atMesh::atMesh(const atMesh &copy) { *this = copy; }

bool atMesh::Validate()
{
  int64_t maxMat = m_materials.size() - 1;
  int64_t maxPos = m_positions.size() - 1;
  int64_t maxColor = m_colors.size() - 1;
  int64_t maxNormal = m_normals.size() - 1;
  int64_t maxTexCoord = m_texCoords.size() - 1;

  // Ensure invalid indices == AT_INVALID_INDEX
  for (Triangle &tri : m_triangles)
  {
    tri.mat = AT_INVALID_INDEX * (tri.mat > maxMat) + (tri.mat <= maxMat) * tri.mat;
    for (int64_t v = 0; v < 3; ++v)
    {
      tri.verts[v].texCoord = AT_INVALID_INDEX * (tri.verts[v].texCoord > maxTexCoord) + (tri.verts[v].texCoord <= maxTexCoord) * tri.verts[v].texCoord;
      tri.verts[v].position = AT_INVALID_INDEX * (tri.verts[v].position > maxPos) + (tri.verts[v].position <= maxPos) * tri.verts[v].position;
      tri.verts[v].normal = AT_INVALID_INDEX * (tri.verts[v].normal > maxNormal) + (tri.verts[v].normal <= maxNormal) * tri.verts[v].normal;
      tri.verts[v].color = AT_INVALID_INDEX * (tri.verts[v].color > maxColor) + (tri.verts[v].color <= maxColor) * tri.verts[v].color;
    }
  }

  for (Triangle &tri : m_triangles)
  {
    for (int64_t v = 0; v < 3; ++v)
    {
      if (tri.verts[v].color == AT_INVALID_INDEX)
      {
        if (m_default.color == AT_INVALID_INDEX)
        {
          m_default.color = m_colors.size();
          m_colors.push_back(atVec4D(1.0, 1.0, 1.0, 1.0));
        }
        tri.verts[v].color = m_default.color;
      }

      if (tri.verts[v].position == AT_INVALID_INDEX)
      {
        if (m_default.position == AT_INVALID_INDEX)
        {
          m_default.position = m_positions.size();
          m_positions.push_back(atVec3D(0.0, 0.0, 0.0));
        }
        tri.verts[v].position = m_default.position;
      }

      if (tri.verts[v].texCoord == AT_INVALID_INDEX)
      {
        if (m_default.texCoord == AT_INVALID_INDEX)
        {
          m_default.texCoord = m_texCoords.size();
          m_texCoords.push_back(atVec2D(0.0, 0.0));
        }
        tri.verts[v].texCoord = m_default.texCoord;
      }
    }

    // Calculate missing normals
    for (int64_t v = 0; v < 3; ++v)
      if (tri.verts[v].normal == AT_INVALID_INDEX)
      {
        tri.verts[v].normal = m_normals.size();
        m_normals.push_back(atVec3D::Cross(
          m_positions[tri.verts[(v + 1) % 3].position] - m_positions[tri.verts[v].position],
          m_positions[tri.verts[(v + 1) % 3].position] - m_positions[tri.verts[(v + 2) % 3].position]));
      }

    if (tri.mat == AT_INVALID_INDEX)
    {
      if (m_defaultMat == AT_INVALID_INDEX)
      {
        m_defaultMat = m_materials.size();
        m_materials.push_back(atMaterial());
      }

      tri.mat = m_defaultMat;
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
  for (atVec3D &pos : m_positions)
    pos = transform * pos;
}

void atMesh::NormalTransform(const atMat4D &transform)
{
  atMat4D nMat = transform.Inverse().Transpose();
  for (atVec3D &norm : m_normals)
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
    for (atFilename &path : mat.m_tBump)
      path = TryDiscoverFile(path.Path(), initialPath);
  }
}

void atMesh::GenTangents()
{
  m_tangents.resize(m_positions.size());
  m_binormals.resize(m_positions.size());
  memset(m_tangents.data(), 0, (size_t)m_tangents.size() * sizeof(atVec3D));
  memset(m_binormals.data(), 0, (size_t)m_binormals.size() * sizeof(atVec3D));

  for (Triangle &tri : m_triangles)
  {
    const atVec3D &v1 = m_positions[tri.verts[0].position];
    const atVec3D &v2 = m_positions[tri.verts[1].position];
    const atVec3D &v3 = m_positions[tri.verts[2].position];
    const atVec2D &t1 = m_texCoords[tri.verts[0].texCoord];
    const atVec2D &t2 = m_texCoords[tri.verts[1].texCoord];
    const atVec2D &t3 = m_texCoords[tri.verts[2].texCoord];

    const atVec3D AB = v2 - v1;
    const atVec3D AC = v3 - v1;
    const atVec2D tAB = t2 - t1;
    const atVec2D tAC = t3 - t1;

    double r = 1.0 / (tAB.x * tAC.y - tAC.x * tAB.y);
    atVec3D uDir = atVec3D{ (tAC.y * AB.x - tAB.y * AC.x), (tAC.y * AB.y - tAB.y * AC.y), (tAC.y * AB.z - tAB.y * AC.z) } * r;
    atVec3D vDir = atVec3D{ (tAB.x * AC.x - tAC.x * AB.x), (tAB.x * AC.y - tAC.x * AB.y), (tAB.x * AC.z - tAC.x * AB.z) } * r;

    for (int64_t i = 0; i < 3; ++i)
    {
      tri.verts[i].tangent = tri.verts[i].position;
      tri.verts[i].bitanget = tri.verts[i].position;
      if (tAB.Mag() > atLimitsSmallest<float>() && tAC.Mag() > atLimitsSmallest<float>())
      {
        m_tangents[tri.verts[i].position] += uDir;
        m_binormals[tri.verts[i].position] += vDir;
      }
      else
      {
        m_tangents[tri.verts[i].position] += 1;
        m_binormals[tri.verts[i].position] += 1;
      }
    }
  }

  for (int64_t i = 0; i < m_positions.size(); ++i)
  {
    m_tangents[i] = m_tangents[i].Normalize();
    m_binormals[i] = m_binormals[i].Normalize();
  }
}

void atMesh::GenNormals()
{
  m_normals.clear();
  atHashMap<atVec3D, int64_t> normMap;
  for (Triangle &tri : m_triangles)
  {
    atVec3D n = (m_positions[tri.verts[2].position] - m_positions[tri.verts[1].position]).Cross(m_positions[tri.verts[0].position] - m_positions[tri.verts[1].position]).Normalize();
    int64_t normIndex = m_normals.size();
    if (!normMap.TryAdd(n, normIndex))
      normIndex = normMap[n];
    else
      m_normals.push_back(n);

    for (int64_t i = 0; i < 3; ++i)
      tri.verts[i].normal = normIndex;
  }
}

void atMesh::GenSmoothNormals(const double threshold, const bool regenNormals)
{
  if (regenNormals)
    GenNormals();
  atVector<atVector<int64_t>> triLookup;
  atVector<atVector<int64_t>> normalLookup;
  triLookup.resize(m_positions.size());
  normalLookup.resize(m_positions.size());
  // Get all connecting triangles for each position
  for (int64_t t = 0; t < m_triangles.size(); ++t)
    for (int64_t v = 0; v < 3; ++v)
      triLookup[m_triangles[t].verts[v].position].push_back(t);

  // Get all normals for each position
  for (int64_t v = 0; v < triLookup.size(); ++v)
  {
    for (const int64_t t : triLookup[v])
    {
      for (int64_t i = 0; i < 3; ++i)
      {
        if (m_triangles[t].verts[i].position == v)
        {
          normalLookup[v].push_back(m_triangles[t].verts[i].normal);
        }
      }
    }
  }

  atVector<atVec3D> normals(m_positions.size());
  double avgDot = 0.0;
  for (int64_t v = 0; v < triLookup.size(); ++v)
  {
    avgDot = 0.0;
    for (const int64_t n : normalLookup[v])
      avgDot += m_normals[normalLookup[v][0]].Dot(m_normals[n]);
    avgDot /= normalLookup[v].size();
    if (avgDot > threshold)
    {
      atVec3D norm = atVec3D::zero();
      for (const int64_t n : normalLookup[v])
        norm += m_normals[n];
      normals.push_back((norm / normalLookup[v].size()).Normalize());

      for (int64_t t : triLookup[v])
        for (int64_t i = 0; i < 3; ++i)
          if (m_triangles[t].verts[i].position == v)
            m_triangles[t].verts[i].normal = normals.size() - 1;
    }
    else
    {
      for (int64_t t : triLookup[v])
        for (int64_t i = 0; i < 3; ++i)
          if (m_triangles[t].verts[i].position == v)
          {
            normals.push_back(m_normals[m_triangles[t].verts[i].normal]);
            m_triangles[t].verts[i].normal = normals.size() - 1;
          }
    }
  }

  m_normals.swap(normals);
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

  // Check if the texture is in a sub folder next to the mesh file
  bool result = false;
  path = atFile::Find(m_resourceDir + "/" + atFilename(file).Name(), &result).Path();
  if (result)
    return path;

  return file;
}

atVector<atTriangle<double>> atMesh::GetTriangles() const
{
  atVector<atTriangle<double>> ret;
  for (const Triangle &tri : m_triangles)
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
int64_t atStreamRead(atReadStream *pStream, atMesh *pData, const int64_t count)
{
  int64_t size = 0;
  for (atMesh &mesh : atIterate(pData, count))
  {
    size += atStreamRead(pStream, &mesh.m_sourceFile, 1);
    size += atStreamRead(pStream, &mesh.m_resourceDir, 1);
    size += atStreamRead(pStream, &mesh.m_default, 1);
    size += atStreamRead(pStream, &mesh.m_triangles, 1);
    size += atStreamRead(pStream, &mesh.m_positions, 1);
    size += atStreamRead(pStream, &mesh.m_normals, 1);
    size += atStreamRead(pStream, &mesh.m_colors, 1);
    size += atStreamRead(pStream, &mesh.m_tangents, 1);
    size += atStreamRead(pStream, &mesh.m_binormals, 1);
    size += atStreamRead(pStream, &mesh.m_texCoords, 1);
    size += atStreamRead(pStream, &mesh.m_materials, 1);
  }
  return size;
}

int64_t atStreamWrite(atWriteStream *pStream, const atMesh *pData, const int64_t count)
{
  int64_t size = 0;
  for (const atMesh &mesh : atIterate(pData, count))
  {
    size += atStreamWrite(pStream, &mesh.m_sourceFile, 1);
    size += atStreamWrite(pStream, &mesh.m_resourceDir, 1);
    size += atStreamWrite(pStream, &mesh.m_default, 1);
    size += atStreamWrite(pStream, &mesh.m_triangles, 1);
    size += atStreamWrite(pStream, &mesh.m_positions, 1);
    size += atStreamWrite(pStream, &mesh.m_normals, 1);
    size += atStreamWrite(pStream, &mesh.m_colors, 1);
    size += atStreamWrite(pStream, &mesh.m_tangents, 1);
    size += atStreamWrite(pStream, &mesh.m_binormals, 1);
    size += atStreamWrite(pStream, &mesh.m_texCoords, 1);
    size += atStreamWrite(pStream, &mesh.m_materials, 1);
  }
  return size;
}

void atMesh::FlipTextures(const bool u, const bool v)
{
  if (!(u || v))
    return;

  for (atVec2D &texCoord : m_texCoords)
  {
    texCoord.x = u ? 1.0 - texCoord.x : texCoord.x;
    texCoord.y = v ? 1.0 - texCoord.y : texCoord.y;
  }
}

void atMesh::FlipNormals() { for (atVec3D &norm : m_normals) norm = -norm; }

void atMesh::Combine(const atMesh &src)
{
  // Material offsets
  int64_t matOffset = m_materials.size();
  m_materials.push_back(src.m_materials);

  // Geometry offsets
  int64_t posOffset = m_positions.size();
  int64_t colOffset = m_colors.size();
  int64_t uvOffset = m_texCoords.size();
  int64_t nrmOffset = m_normals.size();
  int64_t tanOffset = m_tangents.size();
  int64_t bitanOffset = m_binormals.size();
  m_positions.push_back(src.m_positions);
  m_colors.push_back(src.m_colors);
  m_texCoords.push_back(src.m_texCoords);
  m_normals.push_back(src.m_normals);
  m_tangents.push_back(src.m_tangents);
  m_binormals.push_back(src.m_positions);

  // Add all triangles
  m_triangles.reserve(m_triangles.size() + src.m_triangles.size());

  // Adjust indices
  for (const atMesh::Triangle &tri : src.m_triangles)
  {
    m_triangles.push_back(tri);
    atMesh::Triangle &t = m_triangles.back();
    t.mat += matOffset;

    for (int64_t vIdx = 0; vIdx < atArraySize(t.verts); ++vIdx)
    {
      atMesh::Vertex &v = t.verts[vIdx];
      if (v.position != AT_INVALID_INDEX) v.position += posOffset;
      if (v.texCoord != AT_INVALID_INDEX) v.texCoord += uvOffset;
      if (v.bitanget != AT_INVALID_INDEX) v.bitanget += bitanOffset;
      if (v.tangent  != AT_INVALID_INDEX) v.tangent  += tanOffset;
      if (v.normal   != AT_INVALID_INDEX) v.normal   += nrmOffset;
      if (v.color    != AT_INVALID_INDEX) v.color    += colOffset;
    }
  }
}

bool atResourceHandlers::MeshHandler::Load(const atObjectDescriptor &request, atMesh *pResource)
{
  atString path = request["url"].AsString();
  atConstruct(pResource);
  return pResource->Import(path);
}
