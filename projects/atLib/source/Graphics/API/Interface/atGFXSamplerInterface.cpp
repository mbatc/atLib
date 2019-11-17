#include "atGFXSamplerInterface.h"

atGFXSamplerInterface::atGFXSamplerInterface(const atSampleFilter &minFilter, const atSampleFilter &magFilter, const atTexCoordMode &uMode, const atTexCoordMode &vMode, const atTexCoordMode &wMode, const float &mipLodBias, const atComparison &compFunc, const bool &useAnistropicFiltering, const float &minLOD, const float &maxLOD, const atVec4F &borderCol)
  : m_minFilter(minFilter)
  , m_magFilter(magFilter)
  , m_uMode(uMode)
  , m_vMode(vMode)
  , m_wMode(wMode)
  , m_mipLoadBias(mipLodBias)
  , m_compFunc(compFunc)
  , m_minLOD(minLOD)
  , m_maxLOD(maxLOD)
  , m_borderCol(borderCol)
  , m_useAnistropicFiltering(useAnistropicFiltering)
{}
