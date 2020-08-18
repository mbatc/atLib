#include "atAnimation.h"
#include "atTransformable.h"

atAnimation::atAnimation()
{
  m_curves[atAnim_KS_ScaleX].SetDefaultValue(1);
  m_curves[atAnim_KS_ScaleY].SetDefaultValue(1);
  m_curves[atAnim_KS_ScaleZ].SetDefaultValue(1);
}

atAnimationCurve* atAnimation::GetCurve(const atAnimationKeySet &set) { return &m_curves[set]; }

void atAnimation::Evaluate(const atNanoSeconds &time)
{
  for (int64_t i = 0; i < atAnim_KS_Count; ++i)
    m_evaluatedVals[i] = m_curves[i].Evaluate(time);
}

double atAnimation::GetValue(const atAnimationKeySet &set) const { return m_evaluatedVals[set]; }

atMat4D atAnimation::GetTransform() const
{
  atTransformable<double> transform;
  transform.SetTranslation(atVec3D(
    GetValue(atAnim_KS_TranslationX),
    GetValue(atAnim_KS_TranslationY),
    GetValue(atAnim_KS_TranslationZ)));

  transform.SetRotation(atVec3D(
    GetValue(atAnim_KS_RotationX),
    GetValue(atAnim_KS_RotationY),
    GetValue(atAnim_KS_RotationZ)));

  transform.SetScale(atVec3D(
    GetValue(atAnim_KS_ScaleX),
    GetValue(atAnim_KS_ScaleY),
    GetValue(atAnim_KS_ScaleZ)));

  return transform.TransformMat();
}
