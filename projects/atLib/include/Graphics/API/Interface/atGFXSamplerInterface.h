#ifndef atGFXSamplerInterface_h__
#define atGFXSamplerInterface_h__

#include "atGFXResource.h"
#include "atLimits.h"
#include "atMath.h"

enum atTexCoordMode
{
  atTCM_Unknown,
  atTCM_Wrap,           // D3D11_TEXTURE_ADDRESS_WRAP
  atTCM_Mirror,         // D3D11_TEXTURE_ADDRESS_MIRROR
  atTCM_MirrorOnce,     // D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
  atTCM_Clamp,          // D3D11_TEXTURE_ADDRESS_CLAMP
  atTCM_Border          // D3D11_TEXTURE_ADDRESS_BORDER
};

enum atComparison
{
  atComp_Unknown,
  atComp_Equal,         // D3D11_COMPARISON_EQUAL
  atComp_NotEqual,      // D3D11_COMPARISON_NOT_EQUAL
  atComp_Greater,       // D3D11_COMPARISON_GREATER
  atComp_Less,          // D3D11_COMPARISON_LESS
  atComp_GreaterEqual,  // D3D11_COMPARISON_GREATER_EQUAL
  atComp_LessEqual,     // D3D11_COMPARISON_LESS_EQUAL
  atComp_Always,        // D3D11_COMPARISON_ALWAYS
  atComp_Never         // D3D11_COMPARISON_NEVER
};

enum atSampleFilter
{
  atSF_Nearest,
  atSF_Linear,
  atSF_NearestMipmapNearest,
  atSF_LinearMipmapNearest,
  atSF_NearsetMipmapLinear,
  atSF_LinearMipmapLinear,
  atSF_Count,
};

class atGFXSamplerInterface : public atGFXResource
{
public:
  atGFXSamplerInterface(const atSampleFilter &minFilter = atSF_Linear, const atSampleFilter &magFilter = atSF_LinearMipmapLinear, const atTexCoordMode &uMode = atTCM_Wrap, const atTexCoordMode &vMode = atTCM_Wrap, const atTexCoordMode &wMode = atTCM_Wrap, const float &mipLodBias = 0.0f, const atComparison &compFunc = atComp_Never, const bool &useAnistropicFiltering = true, const float &minLOD = 0.0f, const float &maxLOD = atLimitsMax<float>(), const atVec4F &borderCol = { 0, 0, 0, 0 });

protected:
  atSampleFilter m_minFilter = atSF_Linear;
  atSampleFilter m_magFilter = atSF_LinearMipmapLinear;
  atTexCoordMode m_uMode = atTCM_Wrap;
  atTexCoordMode m_vMode = atTCM_Wrap;
  atTexCoordMode m_wMode = atTCM_Wrap;
  atComparison m_compFunc = atComp_Never;
  bool m_useAnistropicFiltering = true;
  float m_mipLoadBias = 0.0f;
  float m_minLOD = 0.0f;
  float m_maxLOD = atLimitsMax<float>();
  atVec4F m_borderCol = { 0, 0, 0, 0 };
};

#endif // atGFXSamplerInterface_h__
