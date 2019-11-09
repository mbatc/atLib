
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

#include "atShader.h"
#include "atFile.h"

static int64_t s_nextLayoutID = 0;

#define PACK_LOC(buffer, var) (int64_t)((int64_t)(buffer) << 32 | (int64_t)(var))
#define LOC_BUFFER(loc) loc >> 32 
#define LOC_VAR(loc) loc & 0xFFFFFFFF

static bool _CompileShader(const atString &src, const atString &shadertype, void **ppShader, ID3D10Blob **ppShaderBlob = nullptr)
{
  if (!ppShader || src.length() == 0)
    return false;

  ID3D10Blob *pShaderBlob = nullptr;
  ID3D10Blob *pError = nullptr;
  if (FAILED(D3DCompile(src.c_str(), src.length(), NULL, NULL, NULL, "main", shadertype, D3D10_SHADER_ENABLE_STRICTNESS, 0, &pShaderBlob, &pError)))
  {
    atAssert(false, (char*)pError->GetBufferPointer());
    return false;
  }

  if (shadertype == VERTEX_SHADER)
    atDirectX::GetDevice()->CreateVertexShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader);
  else if (shadertype == PIXEL_SHADER)
    atDirectX::GetDevice()->CreatePixelShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader);
  else if (shadertype == GEOM_SHADER)
    atDirectX::GetDevice()->CreateGeometryShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader);
  else if (shadertype == HULL_SHADER)
    atDirectX::GetDevice()->CreateHullShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader);
  else if (shadertype == DOMAIN_SHADER)
    atDirectX::GetDevice()->CreateDomainShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader);
  else if (shadertype == COMPUTE_SHADER)
    atDirectX::GetDevice()->CreateComputeShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, (ID3D11ComputeShader**)ppShader);

  if (ppShaderBlob)
    *ppShaderBlob = pShaderBlob;
  else
    atDirectX::SafeRelease(pShaderBlob);
  return *ppShader != nullptr;
}

atShader::~atShader()
{
  atDirectX::SafeRelease(m_pPixel);
  atDirectX::SafeRelease(m_pVert);
  atDirectX::SafeRelease(m_pHull);
  atDirectX::SafeRelease(m_pComp);
  atDirectX::SafeRelease(m_pDomain);
  atDirectX::SafeRelease(m_pGeom);
  m_resource.clear();

  for (auto &kvp : m_layoutLookup)
    kvp.m_val->Release();
  m_layoutLookup.Clear();
}

atShader::atShader(const atString &name)
  : m_pVert(nullptr)
  , m_pPixel(nullptr)
  , m_pHull(nullptr)
  , m_pDomain(nullptr)
  , m_pComp(nullptr)
  , m_pGeom(nullptr)
  , m_reload(false)
{
  atFilename filepath = name;
  atString vertPath = filepath.Path(false) + ".vs";
  atString pixelPath = filepath.Path(false) + ".ps";
  atString hullPath = filepath.Path(false) + ".hs";
  atString geomPath = filepath.Path(false) + ".gs";
  atString computePath = filepath.Path(false) + ".cs";
  atString domainPath = filepath.Path(false) + ".ds";
  SetShaders(pixelPath, vertPath, hullPath, geomPath, computePath, domainPath);
  m_name = name;
}

atShader::atShader(const atString &pixel, const atString &vert, const atString &geometry, const atString &hull, const atString &compute, const atString &domain)
  : m_pVert(nullptr)
  , m_pPixel(nullptr)
  , m_pHull(nullptr)
  , m_pDomain(nullptr)
  , m_pComp(nullptr)
  , m_pGeom(nullptr)
  , m_reload(false) 
{
  SetShadersSource(pixel, vert, geometry, hull, compute, domain);
}

void atShader::Reload()
{
  m_reload = true;
  SetShaders(m_pixelPath, m_vertPath, m_hullPath, m_geomPath, m_computePath, m_domainPath);
}

void atShader::Bind()
{
  UpdateResources();

  atDirectX::GetContext()->VSSetShader(m_pVert, 0, 0);
  atDirectX::GetContext()->PSSetShader(m_pPixel, 0, 0);
  atDirectX::GetContext()->GSSetShader(m_pGeom, 0, 0);
  atDirectX::GetContext()->CSSetShader(m_pComp, 0, 0);
  atDirectX::GetContext()->DSSetShader(m_pDomain, 0, 0);
  atDirectX::GetContext()->HSSetShader(m_pHull, 0, 0);

  for (ResourceDesc &desc : m_resource)
    atGraphics::BindShaderResource(desc.shader, desc.type, desc.reg, desc.pDXResource);
}

void atShader::SetShaders(const atFilename &pixel, const atFilename &vert, const atFilename &hull, const atFilename &geom, const atFilename &compute, const atFilename & domain)
{
  if ((hull != m_hullPath || m_reload) && hull.Path() != "") m_regenBuffers |= SetHullShader(hull);
  if ((vert != m_vertPath || m_reload) && vert.Path() != "") m_regenBuffers |= SetVertexShader(vert);
  if ((geom != m_geomPath || m_reload) && geom.Path() != "") m_regenBuffers |= SetGeometryShader(geom);
  if ((pixel != m_pixelPath || m_reload) && pixel.Path() != "") m_regenBuffers |= SetPixelShader(pixel);
  if ((domain != m_domainPath || m_reload) && domain.Path() != "") m_regenBuffers |= SetDomainShader(domain);
  if ((compute != m_computePath || m_reload) && compute.Path() != "") m_regenBuffers |= SetComputeShader(compute);
  GenerateBuffers();
  m_reload = false;
}

void atShader::SetShadersSource(const atString &pixel, const atString &vert, const atString &hull, const atString &geom, const atString &compute, const atString &domain)
{
  if (hull != "") m_regenBuffers |= SetHullShaderSource(hull);
  if (vert != "") m_regenBuffers |= SetVertexShaderSource(vert);
  if (pixel != "") m_regenBuffers |= SetPixelShaderSource(pixel);
  if (geom != "") m_regenBuffers |= SetGeometryShaderSource(geom);
  if (domain != "") m_regenBuffers |= SetDomainShaderSource(domain);
  if (compute != "") m_regenBuffers |= SetComputeShaderSource(compute);
  GenerateBuffers();
  m_reload = false;
}

bool atShader::SetShader(const atFilename &file, atShaderParser *pParser, atVector<uint8_t> *pByteCode, atFilename *pCurFile, void **ppDXObject, const char *shaderType)
{
  if (!SetShaderSource(atFile::ReadText(file), pParser, pByteCode, pCurFile, ppDXObject, shaderType))
    return false;
  *pCurFile = file;
  return true;
}

bool atShader::SetShaderSource(const atString &src, atShaderParser *pParser, atVector<uint8_t> *pByteCode, atFilename *pCurFile, void **ppDXObject, const char *shaderType)
{
  bool res = true;
  ID3D10Blob *pShader = nullptr;
  if (src != "")
    res = _CompileShader(src, shaderType, ppDXObject, &pShader);
  if (!pShader)
    return res;

  pByteCode->resize(pShader->GetBufferSize());
  memcpy(pByteCode->data(), pShader->GetBufferPointer(), (size_t)pByteCode->size());
  pShader->Release();
  pShader = nullptr;

  if (res)
  {
    *pParser = atShaderParser(src);
    *pCurFile = "";
  }
  return res;
}

void atShader::GenerateBuffers()
{
  if (!m_regenBuffers)
    return;

  m_resource.clear();
  m_bufDirty.clear();
  AddResources(m_vsParser, atST_Vertex);
  AddResources(m_psParser, atST_Pixel);
  AddResources(m_hsParser, atST_Hull);
  AddResources(m_gsParser, atST_Geometry);
  AddResources(m_dsParser, atST_Domain);
  AddResources(m_csParser, atST_Compute);
}

void atShader::AddResources(const atShaderParser &parser, const atShaderType type)
{
  for (auto &buffer : parser.m_cbuffers)
  {
    m_resource.push_back(ResourceDesc());
    m_bufDirty.push_back(true);

    ResourceDesc &rd = m_resource[m_resource.size() - 1];
    rd.data = atVector<uint8_t>(buffer.m_val.actualSize % 16 == 0 ? buffer.m_val.actualSize : (buffer.m_val.actualSize / 16 + 1) * 16, 0);
    rd.offsets = buffer.m_val.packingOffsets;
    rd.vars = buffer.m_val.packingOrder;
    rd.reg = buffer.m_val.reg;
    rd.type = atSRT_Buffer;
    rd.shader = type;

    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = (UINT)rd.data.size();
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA resource;
    resource.pSysMem = rd.data.data();
    resource.SysMemPitch = 0;
    resource.SysMemSlicePitch = 0;

    ID3D11Buffer *pBuffer = nullptr;  
    atAssert(S_OK == atDirectX::GetDevice()->CreateBuffer(&desc, &resource, &pBuffer), "Failed to crate constant buffer!");
    rd.pDXResource = pBuffer;
  }

  for (auto &tex : parser.m_textures)
  {
    m_resource.push_back(ResourceDesc());
    m_bufDirty.push_back(true);
    ResourceDesc &rd = m_resource[m_resource.size() - 1];
    rd.reg = tex.m_key;
    rd.type = atSRT_Texture;
    rd.vars.push_back(tex.m_val);
    rd.offsets.push_back(0);
    rd.shader = type;
  }

  for (auto &smpl : parser.m_samplers)
  {
    m_resource.push_back(ResourceDesc());
    m_bufDirty.push_back(true);
    ResourceDesc &rd = m_resource[m_resource.size() - 1];
    rd.reg = smpl.m_key;
    rd.type = atSRT_Sampler;
    rd.vars.push_back(smpl.m_val);
    rd.offsets.push_back(0);
    rd.shader = type;
  }
}

void atShader::UpdateResources()
{
  ID3D11DeviceContext* pCtx = atDirectX::GetContext();
  for (int64_t i = 0; i < m_resource.size(); ++i)
  {
    if (!m_bufDirty[i])
      continue;

    D3D11_MAPPED_SUBRESOURCE resource;
    switch (m_resource[i].type)
    {
    case atSRT_Buffer:
      pCtx->Map((ID3D11Buffer*)m_resource[i].pDXResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
      memcpy(resource.pData, m_resource[i].data.data(), (size_t)m_resource[i].data.size());
      pCtx->Unmap((ID3D11Buffer*)m_resource[i].pDXResource, 0);
      break;
    case atSRT_Texture: break;
    case atSRT_Sampler: break;
    default: break;
    }
    m_bufDirty[i] = false;
  }
}

int64_t atShader::CreateInputLayout(const atVector<D3D11_INPUT_ELEMENT_DESC> &desc)
{
  ID3D11InputLayout *pLayout = nullptr;
  HRESULT res = atDirectX::GetDevice()->CreateInputLayout(desc.data(), (UINT)desc.size(), m_vsByteCode.data(), (size_t)m_vsByteCode.size(), &pLayout);
  if (FAILED(res))
    return -1;
  m_layoutLookup.Add(s_nextLayoutID, pLayout);
  return s_nextLayoutID++;
}

int64_t atShader::GetResourceLoc(const atString & name)
{
  for (ResourceDesc &desc : m_resource)
    for (const atString &var : desc.vars)
      if (var == name)
        return PACK_LOC(&desc - m_resource.begin(), &var - desc.vars.begin());
  return AT_INVALID_LOC;
}

bool atShader::SetResource(const int64_t loc, void *pData, const int64_t len)
{
  int64_t iBuffer = LOC_BUFFER(loc);
  ResourceDesc &desc = m_resource[iBuffer];
  int64_t varOffset = desc.offsets[LOC_VAR(loc)];

  switch (desc.type)
  {
  case atSRT_Buffer:
    m_bufDirty[iBuffer] = true;
    atAssert(varOffset + len <= desc.data.size(), "pData of [len] will overflow the constant buffer");
    memcpy(desc.data.data() + varOffset, pData, (size_t)len);
    break;
  case atSRT_Texture: case atSRT_Sampler:
    desc.pDXResource = pData;
    break;
  }
  return true;
}

bool atShader::SetResource(const atString &name, void *pData, const int64_t len)
{
  int64_t loc = GetResourceLoc(name);
  if (loc == AT_INVALID_LOC)
    return false;
  SetResource(loc, pData, len);
  return true;
}

const atString& atShader::GetName() const { return m_name; }

bool atShader::SetHullShaderSource(const atString &src)
{
  atDirectX::SafeRelease(m_pHull);
  return SetShaderSource(src, &m_hsParser, &m_hsByteCode, &m_hullPath, (void**)&m_pHull, HULL_SHADER);
}

bool atShader::SetVertexShaderSource(const atString &src)
{
  atDirectX::SafeRelease(m_pVert);
  return SetShaderSource(src, &m_vsParser, &m_vsByteCode, &m_vertPath, (void**)&m_pVert, VERTEX_SHADER);
}

bool atShader::SetPixelShaderSource(const atString &src)
{
  atDirectX::SafeRelease(m_pPixel);
  return SetShaderSource(src, &m_psParser, &m_psByteCode, &m_pixelPath, (void**)&m_pPixel, PIXEL_SHADER);
}

bool atShader::SetGeometryShaderSource(const atString &src)
{
  atDirectX::SafeRelease(m_pGeom);
  return SetShaderSource(src, &m_gsParser, &m_gsByteCode, &m_geomPath, (void**)&m_pGeom, GEOM_SHADER);
}

bool atShader::SetComputeShaderSource(const atString &src)
{
  atDirectX::SafeRelease(m_pComp);
  return SetShaderSource(src, &m_csParser, &m_csByteCode, &m_computePath, (void**)&m_pComp, COMPUTE_SHADER);
}

bool atShader::SetDomainShaderSource(const atString &src)
{
  atDirectX::SafeRelease(m_pDomain);
  return SetShaderSource(src, &m_dsParser, &m_dsByteCode, &m_domainPath, (void**)&m_pDomain, DOMAIN_SHADER);
}

bool atShader::SetVertexShader(const atFilename &file)
{
  atDirectX::SafeRelease(m_pVert);
  return SetShader(file, &m_vsParser, &m_vsByteCode, &m_vertPath, (void**)&m_pVert, VERTEX_SHADER);
}

bool atShader::SetPixelShader(const atFilename &file)
{
  atDirectX::SafeRelease(m_pPixel);
  return SetShader(file, &m_psParser, &m_psByteCode, &m_pixelPath, (void**)&m_pPixel, PIXEL_SHADER);
}

bool atShader::SetHullShader(const atFilename &file)
{
  atDirectX::SafeRelease(m_pHull);
  return SetShader(file, &m_hsParser, &m_hsByteCode, &m_hullPath, (void**)&m_pHull, HULL_SHADER);
}

bool atShader::SetGeometryShader(const atFilename &file)
{
  atDirectX::SafeRelease(m_pGeom);
  return SetShader(file, &m_gsParser, &m_gsByteCode, &m_geomPath, (void**)&m_pGeom, GEOM_SHADER);
}

bool atShader::SetComputeShader(const atFilename &file)
{
  atDirectX::SafeRelease(m_pComp);
  return SetShader(file, &m_csParser, &m_csByteCode, &m_computePath, (void**)&m_pComp, COMPUTE_SHADER);
}

bool atShader::SetDomainShader(const atFilename &file)
{
  atDirectX::SafeRelease(m_pDomain);
  return SetShader(file, &m_dsParser, &m_dsByteCode, &m_domainPath, (void**)&m_pDomain, DOMAIN_SHADER);
}