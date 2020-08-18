#include "atSkeleton.h"
#include "atPose.h"

atPose::atPose(atSkeleton *pSkeleton, const atString &name)
  : m_pSkeleton(pSkeleton)
  , m_name(name)
{}

const atString& atPose::GetName() const { return m_name; }
void atPose::SetName(const atString &name) { m_name = name; }

atMat4D atPose::GetTransform(const int64_t &boneID) const
{
  if (!m_pSkeleton->Contains(boneID))
    return atMat4D::Identity();
  const BindInfo *pTransform = m_boneTransform.TryGet(boneID);
  return pTransform ? pTransform->transform : atMat4D();
}

atMat4D atPose::GetInverse(const int64_t &boneID) const
{
  if (!m_pSkeleton->Contains(boneID))
    return atMat4D::Identity();
  const BindInfo *pTransform = m_boneTransform.TryGet(boneID);
  return pTransform ? pTransform->inverse : atMat4D();
}

bool atPose::SetTransform(const int64_t &boneID, const atMat4D &transform)
{
  if (!m_pSkeleton->Contains(boneID))
    return false;

  BindInfo *pBindInfo = m_boneTransform.TryGet(boneID);
  if (!pBindInfo)
  {
    m_boneTransform.Add(boneID);
    pBindInfo = m_boneTransform.TryGet(boneID);
  }

  pBindInfo->transform = transform;
  pBindInfo->inverse = transform.Inverse();
  return true;
}
