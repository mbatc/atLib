#include "atSampler.h"

void atSampler::SetMinFilter(const atSampleFilter &filter) { m_minFilter = filter; }
void atSampler::SetMagFilter(const atSampleFilter &filter) { m_magFilter = filter; }
void atSampler::SetLODBias(const float &bias) { m_mipLODBias = bias; }
void atSampler::SetLODMin(const float &val) { m_minLOD = val; }
void atSampler::SetLODMax(const float &val) { m_maxLOD = val; }

void atSampler::SetWrapMode(const atTexCoordMode &uMode, const atTexCoordMode &vMode, const atTexCoordMode &wMode)
{
  m_uMode = uMode;
  m_vMode = vMode;
  m_wMode = wMode;
}

void atSampler::SetWrapMode(const atTexCoordMode &mode) { SetWrapMode(mode, mode, mode); }
void atSampler::SetComparison(const atComparison &comp) { m_compFunc = comp; }
void atSampler::SetAnistropicFiltering(const bool &enabled) { m_useAnistropicFiltering = enabled; }
void atSampler::SetBorderColour(const atVec4F &colour) { m_borderCol = colour; }

const atSampleFilter& atSampler::GetMinFilter() const { return m_minFilter; }
const atSampleFilter& atSampler::GetMagFilter() const { return m_magFilter; }
const atTexCoordMode& atSampler::GetWrapModeU() const { return m_uMode; }
const atTexCoordMode& atSampler::GetWrapModeV() const { return m_vMode; }
const atTexCoordMode& atSampler::GetWrapModeW() const { return m_wMode; }
const atComparison& atSampler::GetComparison() const { return m_compFunc; }
const bool& atSampler::GetAnisotropicFiltering() const { return m_useAnistropicFiltering; }
const float& atSampler::GetLODBias() const { return m_mipLODBias; }
const float& atSampler::GetLODMin() const { return m_minLOD; }
const float& atSampler::GetLODMax() const { return m_maxLOD; }
const atVec4F& atSampler::GetBorderColour() const { return m_borderCol; }
