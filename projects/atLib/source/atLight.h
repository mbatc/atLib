#ifndef atLight_h__
#define atLight_h__

#include "atMaterial.h"

enum atLightType
{
  atLT_Directional,
  atLT_Point,
  atLT_Spot
};

class atLight
{
public:
  atLightType m_type;

  atVec3F64 m_position = atVec3F64::zero();
  atVec3F64 m_direction = -atVec3F64::one();
  atVec4F64 m_diffuseColor = atVec4F64::one();
  atVec4F64 m_ambientColor = atVec4F64::zero();
  atVec4F64 m_specularColor = atVec4F64::one();
  
  double m_intensity = 1;
  double m_specularPower = 32;
  double m_coneAngle = atDegs2Rads(45);

  struct Attenuation
  {
    double linear = 0;
    double constant = 0;
    double exponential = 1;
  } m_attenuation;

  // Calculate the frag colour with the lighting applied.
  // only colour channels from the material are used (textures are not applied)
  atVec3F64 CalcLighting(const atVec3F64 &normal, const atVec3F64 &fragPos, const atVec3F64 &camPos = atVec3F64(), const atMaterial &material = atMaterial());

  double CalcLambertian(const atVec3F64 &normal);
  double CalcSpecular(const atVec3F64 &normal, const atVec3F64 &fragPos, const atVec3F64 &camPos);
  double CalcAttenuation(const double dist);
};

#endif // atLight_h__
