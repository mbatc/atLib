#include "atAnimationKey.h"
#include "atMath.h"

double atAnimationKey::GetValue() const { return m_value; }
void atAnimationKey::SetValue(const double &val) { m_value = val; }

atAnimationKey::Interpolation atAnimationKey::GetInterpolation() const { return m_interp; }
void atAnimationKey::SetInterpolation(const atAnimationKey::Interpolation &type) { m_interp = type; }

double atAnimationKey::Interpolate(const atAnimationKey &key, const double &time) const
{
  switch (m_interp)
  {
  case Linear:   return atLerp(GetValue(), key.GetValue(), time);
  case Constant: return m_value;
  }
  return 0.0;
}
