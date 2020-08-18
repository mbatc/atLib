#ifndef atAnimation_h__
#define atAnimation_h__

#include "atMath.h"
#include "atAnimationCurve.h"

enum atAnimationKeySet
{
  atAnim_KS_TranslationX, // Translation
  atAnim_KS_TranslationY,
  atAnim_KS_TranslationZ,
  atAnim_KS_RotationX,    // Rotation
  atAnim_KS_RotationY,
  atAnim_KS_RotationZ,
  atAnim_KS_ScaleX,       // Scale
  atAnim_KS_ScaleY,
  atAnim_KS_ScaleZ,
  atAnim_KS_Count
};

class atAnimation
{
public:
  atAnimation();

  // Get the animation curve for the given key set
  atAnimationCurve* GetCurve(const atAnimationKeySet &set);

  // Evaluate the animation at the specified time
  void Evaluate(const atNanoSeconds &time);

  // Get the evaluated value
  double GetValue(const atAnimationKeySet &set) const;

  atMat4D GetTransform() const;

protected:
  double m_evaluatedVals[atAnim_KS_Count] = { 0 };
  atAnimationCurve m_curves[atAnim_KS_Count];
};

#endif // atAnimation_h__
