
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

atMesh::atMesh() {}

atMesh::atMesh(const atMesh &copy)
{
  m_positions = copy.m_positions;
  m_normals = copy.m_normals;
  m_colors = copy.m_colors;
  m_triangles = copy.m_triangles;
  m_default = copy.m_default;
}

atMesh::atMesh(const atMesh &&move)
  : m_positions(std::move(move.m_positions))
  , m_triangles(std::move(move.m_triangles))
  , m_default(std::move(move.m_default))
  , m_normals(std::move(move.m_normals))
  , m_colors(std::move(move.m_colors))
{}

bool atMesh::MakeValid()
{
  for (Triangle &tri : m_triangles)
  {
    bool calcNormal = false;
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
      calcNormal |= tri.verts[v].normal == AT_INVALID_INDEX;
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
      if (m_default.mat == AT_INVALID_INDEX)
      {
        m_default.mat = m_materials.size();
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

void atMesh::FlipNormals() { for (atVec3F64 &norm : m_normals) norm = -norm; }
void atMesh::FlipTextures(const bool u, const bool v) { if (!(u || v)) return; for (atVec2F64 &texCoord : m_texCoords) texCoord = { u ? 1.0 - texCoord.x : texCoord.x, v ? 1.0 - texCoord.y : texCoord.y }; }
