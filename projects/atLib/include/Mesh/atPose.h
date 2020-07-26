#ifndef atPose_h__
#define atPose_h__

#include "atHashMap.h"
#include "atMath.h"

class atSkeleton;

class atPose
{
  friend atSkeleton;

public:
  atPose(atSkeleton *pSkeleton, const atString &name);

  const atString& GetName() const;
  void SetName(const atString &name);

  atMat4D GetTransform(const int64_t &boneID) const;
  atMat4D GetInverse(const int64_t &boneID) const;
  bool SetTransform(const int64_t &boneID, const atMat4D &transform);

protected:
  atString m_name;

  struct BindInfo
  {
    atMat4D inverse;
    atMat4D transform;
  };

  atHashMap<int64_t, BindInfo> m_boneTransform;
  atSkeleton *m_pSkeleton;
};

#endif // atPose_h__
