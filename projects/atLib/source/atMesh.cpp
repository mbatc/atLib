
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

atMesh::atMesh()
{}

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
      if (tri.vert[v].color == AT_INVALID_INDEX)
      {
        if (m_default.color == AT_INVALID_INDEX)
        {
          m_default.color = m_colors.size();
          m_colors.push_back(atVec4F64(1.0, 1.0, 1.0, 1.0));
        }
        tri.vert[v].color = m_default.color;
      }

      if (tri.vert[v].position == AT_INVALID_INDEX)
      {
        if (m_default.position == AT_INVALID_INDEX)
        {
          m_default.position = m_positions.size();
          m_positions.push_back(atVec3F64(0.0, 0.0, 0.0));
        }
        tri.vert[v].position = m_default.position;
      }
      calcNormal |= tri.vert[v].normal == AT_INVALID_INDEX;
    }

    // calculate missing normals
    for (int64_t v = 0; v < 3; ++v)
      if (tri.vert[v].normal == AT_INVALID_INDEX)
      {
        tri.vert[v].normal = m_normals.size();
        m_normals.push_back(atVectorMath::Cross(m_positions[tri.vert[v].position] - m_positions[tri.vert[(v - 1) % 3].position],
          m_positions[tri.vert[v].position] - m_positions[tri.vert[(v + 1) % 3].position]));
      }

    if (tri.mat == AT_INVALID_INDEX)
      if (m_default.mat == AT_INVALID_INDEX)
      {
        m_default.mat = m_materials.size();
        m_materials.push_back(atMaterial());
      }
  }

  // If there are no triangles this mesh cannot be made valid
  return m_triangles.size() != 0;
}
