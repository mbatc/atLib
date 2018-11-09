#include "atLight.h"

atVec3F64 atLight::CalcLighting(const atVec3F64 &normal, const atVec3F64 &fragPos, const atVec3F64 &camPos, const atMaterial &material)
{
  double lambertian = atMax(0.0, CalcLambertian(normal));
  double sIntensity = 0.0;
  if(m_type != atLT_Directional) lambertian *= CalcAttenuation((m_position - fragPos).Mag());
  if(m_type == atLT_Spot && m_direction.Angle(fragPos - m_position) * 2 > m_coneAngle) lambertian = 0;
  if(lambertian > 0) sIntensity = pow(CalcSpecular(normal, fragPos, camPos), material.m_specularPower * m_specularPower);
  const atVec3F64 &dCol = material.m_cDiffuse.xyz() * material.m_cDiffuse.w;
  const atVec3F64 &sCol = material.m_cSpecular.xyz() * material.m_cSpecular.w;
  const atVec3F64 &aCol = material.m_cAmbient.xyz() * material.m_cAmbient.w;
  return aCol + (m_ambientColor + m_diffuseColor * lambertian) * dCol + sCol * m_specularColor * sIntensity;
}

double atLight::CalcLambertian(const atVec3F64 &normal) { return normal.Dot(-m_direction);  }
double atLight::CalcSpecular(const atVec3F64 &normal, const atVec3F64 &fragPos, const atVec3F64 &camPos) { return (camPos - fragPos).Normalize().Dot(m_direction.Reflect(normal));  }
double atLight::CalcAttenuation(const double dist) { return m_intensity / (m_attenuation.constant + m_attenuation.linear * dist + m_attenuation.exponential * dist * dist); }
