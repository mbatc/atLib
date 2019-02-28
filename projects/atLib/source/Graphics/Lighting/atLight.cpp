
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#include "atLight.h"

atVec3D atLight::CalcLighting(const atVec3D &normal, const atVec3D &fragPos, const atVec3D &camPos, const atMaterial &material)
{
  double lambertian = atMax(0.0, CalcLambertian(normal));
  double sIntensity = 0.0;
  if(m_type != atLT_Directional) lambertian *= CalcAttenuation((m_position - fragPos).Mag());
  if(m_type == atLT_Spot && m_direction.Angle(fragPos - m_position) * 2 > m_coneAngle) lambertian = 0;
  if(lambertian > 0) sIntensity = pow(CalcSpecular(normal, fragPos, camPos), material.m_specularPower * m_specularPower);
  const atVec3D &dCol = material.m_cDiffuse.xyz() * material.m_cDiffuse.w;
  const atVec3D &sCol = material.m_cSpecular.xyz() * material.m_cSpecular.w;
  const atVec3D &aCol = material.m_cAmbient.xyz() * material.m_cAmbient.w;
  const atVec4D &difLight = (m_ambientColor + m_diffuseColor * lambertian);
  return aCol + difLight.xyz() * difLight.w * dCol + sCol * (m_specularColor.xyz() * m_specularColor.w) * sIntensity;
}

double atLight::CalcLambertian(const atVec3D &normal) { return normal.Dot(-m_direction);  }
double atLight::CalcSpecular(const atVec3D &normal, const atVec3D &fragPos, const atVec3D &camPos) { return (camPos - fragPos).Normalize().Dot(m_direction.Reflect(normal));  }
double atLight::CalcAttenuation(const double dist) { return m_intensity / (m_attenuation.constant + m_attenuation.linear * dist + m_attenuation.exponential * dist * dist); }
