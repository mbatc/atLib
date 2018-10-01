
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

#ifndef _atMesh_h__
#define _atMesh_h__

#include "atVector.h"
#include "atMath.h"
#include "atMaterial.h"

class atMesh
{
public:
  struct Vertex
  {
    int64_t position = AT_INVALID_INDEX;
    int64_t normal = AT_INVALID_INDEX;
    int64_t color = AT_INVALID_INDEX;
  };

  struct Triangle
  {
    int64_t mat;
    Vertex vert[3];
  };
    
  atMesh();
  atMesh(const atMesh &copy);
  atMesh(const atMesh &&move);

  // These values will be used to fill in missing data when processing the mesh
  struct
  {
    int64_t color = AT_INVALID_INDEX;
    int64_t position = AT_INVALID_INDEX;
    int64_t mat = AT_INVALID_INDEX;
  } m_default;

  bool MakeValid();

  atVector<Triangle> m_triangles;
  atVector<atVec3F64> m_positions;
  atVector<atVec3F64> m_normals;
  atVector<atVec4F64> m_colors;
  atVector<atMaterial> m_materials;
};

#endif