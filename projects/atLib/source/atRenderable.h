
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

enum atRenderable_ResourceType
{
  atRRT_None = 1 << 0,
  atRRT_Texture = 1 << 1,
  atRRT_Sampler = 1 << 2,
  atRRT_Variable = 1 << 3,
  atRRT_VertexData = 1 << 4,
  atRRT_Indices = 1 << 5,
  atRRT_All = INT_MAX
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
  struct Resource
  {
    atRenderable_ResourceType type = atRRT_None;
    atTypeDesc desc = atTypeDesc{ atType_Unknown, 0 };
    int64_t count = 0;
    int64_t loc = AT_INVALID_ID;
    int64_t id = AT_INVALID_ID;
    atVector<uint8_t> data;
  };


  atRenderable();
  atRenderable(atRenderable &&move);
  ~atRenderable();
  
  void SetShader(const atString &name);
  
  bool Draw(const atMat4 &mvp, const atRenderable_PrimitiveType type = atRPT_TriangleList);
  void Clear();

  template <typename T> void SetChannel(const atString &name, const T &data, const atRenderable_ResourceType type);
  template <typename T> void SetChannel(const atString &name, const atVector<T> &data, const atRenderable_ResourceType type);
  template <typename T> void SetChannel(const atString &name, const atStringBasic<T> &data, const atRenderable_ResourceType type);
  template <typename T> void SetChannel(const atString &name, const std::initializer_list<T> &list, const atRenderable_ResourceType type);

  // Per Vertex Data
  // atHashMap<atString, atVector<atVec3F>> m_positions;
  // atHashMap<atString, atVector<atVec3F>> m_normals;
  // atHashMap<atString, atVector<atVec4F>> m_colors;
  // atHashMap<atString, atVector<atVec2F>> m_texCoords;
  // atVector<uint32_t> m_indices;

  // Textures
  // atHashMap<atString, int64_t> m_textures;
  // atHashMap<atString, int64_t> m_samplers;

  Resource& GetResource(const atString &name);
  void FreeResource(const atString &name);

protected:
  bool Rebuild();

  atHashMap<atString, Resource> m_resource;

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

template<typename T> void atRenderable::SetChannel(const atString &name, const T &data, const atRenderable_ResourceType type)
{
  Resource &res = GetResource(name);
  res = Resource();
  res.count = 1;
  res.desc = atGetTypeDesc<T>();
  res.type = type;
  res.data.resize(res.desc.size);
  memcpy(res.data.data(), &data, res.data.size());
}

template<typename T> void atRenderable::SetChannel(const atString &name, const atVector<T> &data, const atRenderable_ResourceType type)
{
  Resource &res = GetResource(name);
  res = Resource();
  res.desc = atGetTypeDesc<T>();
  res.count = data.size();
  res.type = type;
  res.data.resize(res.desc.size * res.count);
  memcpy(res.data.data(), data.data(), res.data.size());
}

template<typename T> void atRenderable::SetChannel(const atString &name, const atStringBasic<T>& data, const atRenderable_ResourceType type)
{
  Resource &res = GetResource(name);
  res = Resource();
  res.desc = atGetTypeDesc<T>();
  res.count = data.length() + 1;
  res.type = type;
  res.data.resize(res.count * res.desc.size);
  memcpy(res.data.data(), data.c_str(), res.data.size());
}

template<typename T> void atRenderable::SetChannel(const atString & name, const std::initializer_list<T>& list, const atRenderable_ResourceType type) { SetChannel(name, atVector<T>(list), type); }

#endif
