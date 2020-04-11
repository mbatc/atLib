#ifndef atGFXSamplerInterface_h__
#define atGFXSamplerInterface_h__

#include "atGFXResource.h"
#include "atLimits.h"
#include "atMath.h"

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
