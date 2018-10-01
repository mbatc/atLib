
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

#include "atShaderPool.h"
#include "atShader.h"
#include "atHashMap.h"
#include "atFormat.h"

static int64_t s_nextShaderID = 0;
static int64_t s_boundShader = AT_INVALID_ID;

struct ShaderReference
{
  ShaderReference(atShader* pShader)
    : pShader(pShader)
    , refCount(0)
  {}
  atShader *pShader;
  int64_t refCount;
};

static int64_t s_shaderRound = 0;
static atHashMap<int64_t, ShaderReference> s_shaders;

int64_t atShaderPool::GetShader(const atString &name)
{
  for (auto &ref : s_shaders)
    if (ref.m_val.pShader->GetName() == name)
    {
      ref.m_val.refCount++;
      return ref.m_key;
    }

  s_shaders.Add(s_nextShaderID, ShaderReference(atNew<atShader>(name)));
  s_shaders[s_nextShaderID].refCount++;
  return s_nextShaderID++;
}

int64_t atShaderPool::BindShader(const int64_t id)
{
  s_boundShader = AT_INVALID_ID;

  ShaderReference *pRef = s_shaders.TryGet(id);
  if (pRef)
  {
    pRef->pShader->Bind();
    s_boundShader = id;
    return id;
  }

  atGraphics::GetContext()->VSSetShader(nullptr, 0, 0);
  atGraphics::GetContext()->PSSetShader(nullptr, 0, 0);
  atGraphics::GetContext()->GSSetShader(nullptr, 0, 0);
  atGraphics::GetContext()->CSSetShader(nullptr, 0, 0);
  atGraphics::GetContext()->DSSetShader(nullptr, 0, 0);
  atGraphics::GetContext()->HSSetShader(nullptr, 0, 0);
  atGraphics::GetContext()->IASetInputLayout(nullptr);
  return AT_INVALID_ID;
}

int64_t atShaderPool::BindInputLayout(const int64_t id)
{
  ShaderReference *pRef = s_shaders.TryGet(s_boundShader);
  if (!pRef)
    return AT_INVALID_ID;
  
  ID3D11InputLayout **ppLayout = pRef->pShader->m_layoutLookup.TryGet(id);
  if (!ppLayout)
    return AT_INVALID_ID;
  
  atGraphics::GetContext()->IASetInputLayout(*ppLayout);
  return id;
}

int64_t atShaderPool::GetInputLayout(const int64_t id, const atVector<VertexData> &desc)
{
  ShaderReference *pRef = s_shaders.TryGet(id);
  if (!pRef)
    return AT_INVALID_ID;

#ifdef _DEBUG
  for (auto &kvp : pRef->pShader->m_vsParser.m_inputs)
  {
    bool found = false;
    for (const VertexData &vd : desc)
    {
      if (vd.semantic != kvp.m_key)
        continue;
      found = true;
      break;
    }
    atAssert(found, ("Vertex Data Description is not compatible with " + pRef->pShader->m_vertPath.Name() + " (Expects Semantic '" + kvp.m_key + "')").c_str());
  }
#endif

  atVector<D3D11_INPUT_ELEMENT_DESC> d3dDesc;
  d3dDesc.reserve(desc.size());
  for (const VertexData &data : desc)
    d3dDesc.push_back({data.semantic.c_str(), 0, (DXGI_FORMAT)atFormat::DXGI(data.desc.type, data.desc.count), 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0});
  return pRef->pShader->CreateInputLayout(d3dDesc);
}

bool atShaderPool::SetVariable(const int64_t shader, const atString &name, void *pData, int64_t len)
{
  ShaderReference *pRef = s_shaders.TryGet(shader);
  return pRef ? pRef->pShader->SetResource(name, pData, len) : false;
}

void atShaderPool::ReleaseShader(const int64_t id)
{
  ShaderReference *pRef = s_shaders.TryGet(id);
  if (!pRef || --pRef->refCount != 0)
    return;
  atDelete(pRef->pShader);
  s_shaders.Remove(id);
  return;
}

void atShaderPool::ReloadShaders()
{
  for (auto &ref : s_shaders)
    ref.m_val.pShader->Reload();
  s_shaderRound++;
}

int64_t atShaderPool::ShaderRound() { return s_shaderRound; }

// A hacky way of releasing resources when the program ends
struct CleanupStruct
{
  ~CleanupStruct()
  {
    for (auto &kvp : s_shaders)
      atDelete(kvp.m_val.pShader);
  }
};

static CleanupStruct cleanup;
