#ifndef atGFXSamplerInterface_h__
#define atGFXSamplerInterface_h__

#include "atGFXResource.h"
#include "atLimits.h"
#include "atMath.h"

class atSampler : public atGFXResource
{
public:
  atSampler() = default;

  // Minification filter
  void SetMinFilter(const atSampleFilter &filter);

  // Magnification filter
  void SetMagFilter(const atSampleFilter &filter);

  // Mip-map layer bias
  void SetLODBias(const float &bias);
  
  // Minimum mip-map layer
  void SetLODMin(const float &val);

  // Maximum mip-map layer
  void SetLODMax(const float &val);

  // Texture coordinate wrapping mode
  void SetWrapMode(const atTexCoordMode &uMode, const atTexCoordMode &vMode, const atTexCoordMode &wMode);
  void SetWrapMode(const atTexCoordMode &mode);

  // Set texel comparison mode
  void SetComparison(const atComparison &comp);

  // Enable anisotropic filtering
  void SetAnistropicFiltering(const bool &enabled);

  // Set texture border colour
  void SetBorderColour(const atVec4F &colour);

  const atSampleFilter& GetMinFilter() const;
  const atSampleFilter& GetMagFilter() const;
  const atTexCoordMode& GetWrapModeU() const;
  const atTexCoordMode& GetWrapModeV() const;
  const atTexCoordMode& GetWrapModeW() const;
  const atComparison& GetComparison() const;
  const bool& GetAnisotropicFiltering() const;
  const float& GetLODBias() const;
  const float& GetLODMin() const;
  const float& GetLODMax() const;
  const atVec4F& GetBorderColour() const;

protected:
  atSampleFilter m_minFilter = atSF_Linear;
  atSampleFilter m_magFilter = atSF_LinearMipmapLinear;
  atTexCoordMode m_uMode = atTCM_Wrap;
  atTexCoordMode m_vMode = atTCM_Wrap;
  atTexCoordMode m_wMode = atTCM_Wrap;
  atComparison m_compFunc = atComp_Never;
  bool m_useAnistropicFiltering = true;
  float m_mipLODBias = 0.0f;
  float m_minLOD = 0.0f;
  float m_maxLOD = atLimitsMax<float>();
  atVec4F m_borderCol = { 0, 0, 0, 0 };
};

#endif // atGFXSamplerInterface_h__
