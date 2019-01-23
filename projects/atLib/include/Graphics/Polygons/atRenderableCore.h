
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

#ifndef _atRenderableCore_h__
#define _atRenderableCore_h__

#include "atMesh.h"
#include "atHashMap.h"
#include "atGraphics.h"
#include "atMath.h"

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

enum atRenderable_ResourceType
{
  atRRT_None,
  atRRT_Texture,
  atRRT_Sampler,
  atRRT_Variable,
  atRRT_VertexData,
  atRRT_Indices
};

struct VertexData
{
  atString semantic;
  atTypeDesc desc;
};

class atShader;

class atRenderableCore
{
public:
  struct Resource
  {
    atRenderable_ResourceType type = atRRT_None;
    atTypeDesc desc = atTypeDesc{ atType_Unknown, 0 };
    int64_t count = 0;
    int64_t loc = AT_INVALID_ID;
    int64_t id = AT_INVALID_ID;
    atVector<uint8_t> data;
  };


  atRenderableCore();
  atRenderableCore(atRenderableCore &&move);
  atRenderableCore(const atRenderableCore &copy);
  ~atRenderableCore();
  
  void SetShader(const atString &name);
  
  bool Draw(const atRenderable_PrimitiveType type = atRPT_TriangleList);
  void Clear();

  template <typename T> void SetChannel(const atString &name, const T &data, const atRenderable_ResourceType type);
  template <typename T> void SetChannel(const atString &name, const atVector<T> &data, const atRenderable_ResourceType type);
  template <typename T> void SetChannel(const atString &name, const atStringBasic<T> &data, const atRenderable_ResourceType type);
  template <typename T> void SetChannel(const atString &name, const std::initializer_list<T> &list, const atRenderable_ResourceType type);

  bool HasResource(const atString &name);
  Resource& GetResource(const atString &name);
  void FreeResource(const atString &name);

protected:
  bool Rebuild();

  atHashMap<atString, Resource> m_resource;

  atVector<VertexData> m_layout;
  int64_t m_stride;

  atString m_shader;
  int64_t m_shaderID;
  int64_t m_layoutID;
  
  ID3D11Buffer *m_pVertBuffer;
  ID3D11Buffer *m_pIndexBuffer;
  
  int64_t m_nIndices;
  int64_t m_nVerts;
  int64_t m_shaderRound;
};

#include "atRenderableCore.inl"
#endif

