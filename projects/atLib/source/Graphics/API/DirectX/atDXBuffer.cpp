#ifdef atPLATFORM_WIN32

#include "atDXBuffer.h"
#include "atDirectX.h"
#include "atGraphics.h"
#include "atDXInclude_Internal.h"

atDXBuffer::atDXBuffer(const atBufferType &type, const int64_t &size)
  : atGPUBuffer(type)
{
  Resize(size);
}

bool atDXBuffer::Upload()
{
  if (NativeResource() && m_bufferSize <= m_gpuBufferSize)
    return true;

  Delete();
  if (m_bufferSize <= 0)
    return false;

  D3D11_BUFFER_DESC desc = { 0 };
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.Usage = D3D11_USAGE_DYNAMIC;
  UINT cpuAccess = 0;
  cpuAccess = D3D11_CPU_ACCESS_WRITE;
  
  switch (m_mapping)
  {
  case atBT_VertexData: desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; break;
  case atBT_IndexData: desc.BindFlags = D3D11_BIND_INDEX_BUFFER; break;
  case atBT_ShaderData: desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; break;
  default: return false;
  }

  desc.ByteWidth = (UINT)m_bufferSize;
  desc.CPUAccessFlags = cpuAccess;

  ID3D11Buffer *pBuffer = nullptr;
  bool success = !FAILED(((ID3D11Device*)((atDirectX*)atGraphics::GetCurrent())->GetDevice())->CreateBuffer(&desc, nullptr, &pBuffer));
  m_pResource = pBuffer;
  m_gpuBufferSize = success ? m_bufferSize : 0;
  return success;
}

bool atDXBuffer::Delete()
{
  if (!m_pResource)
    return false;
  ID3D11Buffer *pBuffer = (ID3D11Buffer*)m_pResource;
  atDirectX::SafeRelease(pBuffer);
  m_pResource = nullptr;
  m_gpuBufferSize = 0;
  return true;
}

bool atDXBuffer::Resize(const int64_t &size)
{
  m_bufferSize = size;
  return Upload();
}

void *atDXBuffer::Map(const atGPUBuffer_MapFlags &flags)
{
  if (m_bufferSize == 0)
    return nullptr;

  if (!m_pMappedPtr)
  {
    atDirectX *pDX = (atDirectX *)atGraphics::GetCurrent();
    ID3D11Buffer *pBuffer = (ID3D11Buffer *)NativeResource();
    ID3D11DeviceContext *pCtx = (ID3D11DeviceContext *)pDX->GetContext();
    D3D11_MAPPED_SUBRESOURCE resource;

    pCtx->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
    if (!resource.pData)
      return nullptr;

    ++m_mappedCount;
    m_pMappedPtr = resource.pData;
  }

  return m_pMappedPtr;
}

bool atDXBuffer::Unmap()
{
  if (!m_pMappedPtr)
    return false;

  atDirectX *pDX = (atDirectX *)atGraphics::GetCurrent();
  ID3D11Buffer *pBuffer = (ID3D11Buffer *)NativeResource();
  ID3D11DeviceContext *pCtx = (ID3D11DeviceContext *)pDX->GetContext();

  if (--m_mappedCount == 0)
  {
    pCtx->Unmap(pBuffer, 0);
    m_pMappedPtr = nullptr;
  }

  return true;
}

bool atDXBuffer::Resize(const int64_t &size)
{
  m_bufferSize = size;
  return Upload();
}

void *atDXBuffer::Map(const atGPUBuffer_MapFlags &flags)
{
  if (m_bufferSize == 0)
    return nullptr;

  atDirectX *pDX = (atDirectX *)atGraphics::GetCurrent();
  ID3D11Buffer *pBuffer = (ID3D11Buffer *)NativeResource();
  ID3D11DeviceContext *pCtx = (ID3D11DeviceContext *)pDX->GetContext();
  D3D11_MAPPED_SUBRESOURCE resource;

  pCtx->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
  if (!resource.pData)
    return nullptr;

  ++m_mappedCount;
  m_pMappedPtr = resource.pData;
  return m_pMappedPtr;
}

bool atDXBuffer::Unmap()
{
  if (!m_pMappedPtr)
    return false;

  atDirectX *pDX = (atDirectX *)atGraphics::GetCurrent();
  ID3D11Buffer *pBuffer = (ID3D11Buffer *)NativeResource();
  ID3D11DeviceContext *pCtx = (ID3D11DeviceContext *)pDX->GetContext();

  if (--m_mappedCount == 0)
  {
    pCtx->Unmap(pBuffer, 0);
    m_pMappedPtr = nullptr;
  }

  return true;
}

#else

bool atDXBuffer::Update() { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
bool atDXBuffer::Upload() { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
bool atDXBuffer::Delete() { atRelAssert("DirectX is only supported on Windows platforms."); return false; }

#endif
