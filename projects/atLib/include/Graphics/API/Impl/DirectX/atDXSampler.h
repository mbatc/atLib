#ifndef atDXSampler_h__
#define atDXSampler_h__

#include "atGFXSamplerInterface.h"

class atDXSampler : public atGFXSamplerInterface
{
public:
  atDXSampler(const atSampleFilter &minFilter = atSF_Linear, const atSampleFilter &magFilter = atSF_LinearMipmapLinear, const atTexCoordMode &uMode = atTCM_Wrap, const atTexCoordMode &vMode = atTCM_Wrap, const atTexCoordMode &wMode = atTCM_Wrap, const float &mipLodBias = 0.0f, const atComparison &compFunc = atComp_Never, const bool &useAnistropicFiltering = true, const float &minLOD = 0.0f, const float &maxLOD = atLimitsMax<float>(), const atVec4F &borderCol = { 0, 0, 0, 0 });
  
  atGraphicsAPI API() override { return atGfxApi_DirectX; }

  bool Upload() override;
  bool Delete() override;
};

#endif // atDXSampler_h__
