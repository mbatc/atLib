#ifdef atPLATFORM_WIN32

#include "atDXSampler.h"
#include "atGraphics.h"
#include "atDirectX.h"

static int64_t _GetDXFilter(const atSampleFilter &minFilter, const atSampleFilter &magFilter)
{
  switch (magFilter)
  {
  case atSF_Linear:
    switch (minFilter)
    {
    case atSF_Linear: return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    case atSF_Nearest: return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
    case atSF_LinearMipmapLinear: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    case atSF_LinearMipmapNearest: return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    case atSF_NearsetMipmapLinear: return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
    case atSF_NearestMipmapNearest: return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
    default: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    }
  case atSF_Nearest:
    switch (minFilter)
    {
    case atSF_Linear: return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
    case atSF_Nearest: return D3D11_FILTER_MIN_MAG_MIP_POINT;
    case atSF_LinearMipmapLinear: return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
    case atSF_LinearMipmapNearest: return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
    case atSF_NearsetMipmapLinear: return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
    case atSF_NearestMipmapNearest: return D3D11_FILTER_MIN_MAG_MIP_POINT;
    default: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    }
  default: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  }
}

static int64_t _GetWrapMode(const atTexCoordMode &wrapMode)
{
  switch (wrapMode)
  {
  case atTCM_Mirror:      return D3D11_TEXTURE_ADDRESS_MIRROR;
  case atTCM_MirrorOnce:  return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
  case atTCM_Clamp:       return D3D11_TEXTURE_ADDRESS_CLAMP;
  case atTCM_Border:      return D3D11_TEXTURE_ADDRESS_BORDER;
  default:
  case atTCM_Wrap:        return D3D11_TEXTURE_ADDRESS_WRAP;
  }
}

static int64_t _GetComparison(const atComparison &wrapMode)
{
  switch (wrapMode)
  {
  case atComp_Equal:        return D3D11_COMPARISON_EQUAL;
  case atComp_NotEqual:     return D3D11_COMPARISON_NOT_EQUAL;
  case atComp_Greater:      return D3D11_COMPARISON_GREATER;
  case atComp_Less:         return D3D11_COMPARISON_LESS;
  case atComp_GreaterEqual: return D3D11_COMPARISON_GREATER_EQUAL;
  case atComp_LessEqual:    return D3D11_COMPARISON_LESS_EQUAL;
  case atComp_Always:       return D3D11_COMPARISON_ALWAYS;
  default:
  case atComp_Never:        return D3D11_COMPARISON_NEVER;
  }
}

atDXSampler::atDXSampler(const atSampleFilter &minFilter, const atSampleFilter &magFilter, const atTexCoordMode &uMode, const atTexCoordMode &vMode, const atTexCoordMode &wMode, const float &mipLodBias, const atComparison &compFunc, const bool &useAnistropicFiltering, const float &minLOD, const float &maxLOD, const atVec4F &borderCol)
  : atGFXSamplerInterface(minFilter, magFilter, uMode, vMode, wMode, mipLodBias, compFunc, useAnistropicFiltering, minLOD, maxLOD, borderCol)
{}

bool atDXSampler::Upload()
{
  D3D11_SAMPLER_DESC desc;
  desc.Filter = (D3D11_FILTER)_GetDXFilter(m_minFilter, m_magFilter);
  desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)_GetWrapMode(m_uMode);
  desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)_GetWrapMode(m_vMode);
  desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)_GetWrapMode(m_wMode);
  desc.ComparisonFunc = (D3D11_COMPARISON_FUNC)_GetComparison(m_compFunc);
  desc.MaxLOD = m_maxLOD;
  desc.MinLOD = m_minLOD;
  desc.MipLODBias = m_mipLoadBias;
  desc.MaxAnisotropy = m_useAnistropicFiltering ? 1 : 0;
  memcpy(desc.BorderColor, m_borderCol.data(), (size_t)atGetTypeDesc(m_borderCol).size);
  ((atDirectX*)atGraphics::GetCtx())->GetDevice()->CreateSamplerState(&desc, (ID3D11SamplerState**)&m_pResource);
  return m_pResource != nullptr;
}

bool atDXSampler::Delete()
{
  if (!GFXResource())
    return false;

  ID3D11SamplerState *pState;
  atDirectX::SafeRelease(pState);
  m_pResource;
  return true;
}

#endif
