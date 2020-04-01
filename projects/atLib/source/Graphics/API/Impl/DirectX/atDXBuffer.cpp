#ifdef atPLATFORM_WIN32

#include "atDXBuffer.h"
#include "atDirectX.h"
#include "atGraphics.h"

bool atDXBuffer::Update()
{
  if (!GFXResource())
    return false;

  atDirectX *pDX = (atDirectX*)atGraphics::GetCtx();
  ID3D11Buffer *pBuffer = (ID3D11Buffer*)GFXResource();
  D3D11_BUFFER_DESC desc;
  pBuffer->GetDesc(&desc);
  if (desc.ByteWidth != Size())
  {
    Delete();
    return Upload();
  }

  D3D11_MAPPED_SUBRESOURCE resource;
  pDX->GetContext()->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

  if (!resource.pData)
    return false;

  memcpy(resource.pData, Data().data(), Size());
  pDX->GetContext()->Unmap(pBuffer, 0);
  return true;
}

bool atDXBuffer::Upload()
{
  if (GFXResource())
    return true;

  D3D11_BUFFER_DESC desc = { 0 };
  desc.Usage = D3D11_USAGE_DEFAULT;
  
  UINT cpuAccess = 0;
  switch (m_mapping)
  {
  case atBT_VertexData:
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    cpuAccess = D3D11_CPU_ACCESS_WRITE;

    break;
  
  case atBT_IndexData:
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    cpuAccess = D3D11_CPU_ACCESS_WRITE;
    break;
  
  case atBT_ShaderData:
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    cpuAccess = D3D11_CPU_ACCESS_WRITE;
    break;

  default: return false;
  }

  int64_t size = Data().size();
  desc.ByteWidth = (UINT)size;
  desc.CPUAccessFlags = cpuAccess;

  D3D11_SUBRESOURCE_DATA data = { 0 };
  data.pSysMem = Data().data();
  data.SysMemPitch = 0;
  data.SysMemSlicePitch = 0;

  ID3D11Buffer *pBuffer = nullptr;
  bool success = !FAILED(((atDirectX*)atGraphics::GetCtx())->GetDevice()->CreateBuffer(&desc, size > 0 ? &data : nullptr, &pBuffer));
  m_pResource = pBuffer;
  return success;
}

bool atDXBuffer::Delete()
{
  if (!m_pResource)
    return false;
  ID3D11Buffer *pBuffer = (ID3D11Buffer*)m_pResource;
  atDirectX::SafeRelease(pBuffer);
  m_pResource = nullptr;
  return true;
}

#endif
