
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

#ifndef _atRenderable_h__
#define _atRenderable_h__

#include "atMesh.h"
#include "atHashMap.h"
#include "atGraphics.h"

enum atRenderable_PrimitiveType
{
  atRPT_TriangleListAdj,
  atRPT_TriangleList,
  atRPT_TriangleStrip,
  atRPT_LineListAdj,
  atRPT_LineList,
  atRPT_PointListAdj,
  atRPT_PointList,
};

struct VertexData
{
  atString semantic;
  atTypeDesc desc;
  // int64_t semanticIndex = 0;
};

class atShader;

class atRenderable
{
public:
  atRenderable();
  atRenderable(atRenderable &&move);
  ~atRenderable();
  
  void SetShader(const atString &name);
  
  bool Draw(const atMat4 &mvp, const atRenderable_PrimitiveType type = atRPT_TriangleList);
  bool Rebuild();
  void Clear();

  // Per Vertex Data
  atHashMap<atString, atVector<atVec3F>> m_positions;
  atHashMap<atString, atVector<atVec3F>> m_normals;
  atHashMap<atString, atVector<atVec4F>> m_colors;
  atHashMap<atString, atVector<atVec2F>> m_texCoords;
  atVector<uint32_t> m_indices;

  // Textures
  atHashMap<atString, int64_t> m_textures;

protected:
  atVector<VertexData> m_layout;
  int64_t m_stride;

  ID3D11Buffer *m_pVertBuffer;
  ID3D11Buffer *m_pIndexBuffer;

  int64_t m_shaderID;
  int64_t m_layoutID;
  int64_t m_nIndices;
  int64_t m_nVerts;
  int64_t m_shaderRound;

  atString m_shader;
};

#endif