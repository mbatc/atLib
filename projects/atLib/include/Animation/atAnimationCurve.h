#ifndef atAnimationCurve_h__
#define atAnimationCurve_h__

#include "atTimespan.h"
#include "atAnimationKey.h"
#include "atTimespan.h"
#include "atVector.h"
#include "atKeyValue.h"

class atAnimationCurve
{
public:
  // Get the number of keyframes in the curve
  int64_t GetKeyCount() const;

  // Get the keyframe an the specified index
  const atAnimationKey& GetKey(const int64_t &index) const;

  // Get the time of a keyframe at the specified index
  atNanoSeconds GetKeyTime(const int64_t &index) const;

  // Set the a keys position in the time line using a time value.
  // Keys are stored in ascending order in time so the index of the
  // Key may change.
  // The final index of the key is returned.
  int64_t SetKeyTime(const int64_t &index, const atNanoSeconds &time);

  // Set an animation key by index
  void SetKey(const int64_t &index, const atAnimationKey &key);

  // Set an animation key by time.
  // If the key does not exist it will be added.
  int64_t SetKey(const atNanoSeconds &time, const atAnimationKey &key);

  // Evaluate the curve at the specified time.
  // Optionally pass the last keyframe index that was evaluated.
  double Evaluate(const atNanoSeconds &time, int64_t *pLastIndex = nullptr) const;

  // Find the keyframe (or closest keyframe) at the specified time.
  // Optionally pass the last keyframe index that was evaluated.
  int64_t Find(const atNanoSeconds &time, int64_t *pLastIndex = nullptr) const;

  bool Remove(const int64_t &index);

  void SetDefaultValue(const double &value);

protected:
  double m_defaultValue = 0;
  atVector<atKeyValue<atNanoSeconds, atAnimationKey>> m_keys;
};

#endif // atAnimationCurve_h__
