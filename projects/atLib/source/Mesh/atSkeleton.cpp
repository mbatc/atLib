#include "atSkeleton.h"

int64_t atSkeleton::GetPoseCount() const { return m_poses.size(); }

atPose& atSkeleton::GetPose(const int64_t &poseIndex)
{
  atAssert(poseIndex >= 0 && poseIndex < m_poses.size(), "poseIndex is not valid.");
  return m_poses[poseIndex];
}

const atPose& atSkeleton::GetPose(const int64_t &poseIndex) const
{
  atAssert(poseIndex >= 0 && poseIndex < m_poses.size(), "poseIndex is not valid.");
  return m_poses[poseIndex];
}

void atSkeleton::RemovePose(const int64_t &index)
{
  atAssert(index >= 0 && index < m_poses.size(), "poseIndex is not valid.");
  m_poses.erase(index);
}

void atSkeleton::SetTransform(const int64_t &boneID, const atMat4D &transform)
{
  atAssert(Contains(boneID), "The skeleton does not contain the specified bone");
  atBone &bone = Get(boneID);
  bone.localTransform = transform;
  FlagModified(boneID);
}

void atSkeleton::SetGlobalTransform(const int64_t &boneID, const atMat4D &transform)
{
  atAssert(Contains(boneID), "The skeleton does not contain the specified bone");
  EvaluateGlobalTransform(boneID);
  atBone &bone = Get(boneID);

  // Find the transform 'offset' between the current global transform and the target global transform
  atMat4D offset = bone.globalTransform.Inverse() * transform;

  // Apply the offset to the current local transform
  bone.localTransform = offset * bone.localTransform;

  // Set the global transform
  bone.globalTransform = transform;
  FlagModified(boneID);
}

atMat4D atSkeleton::EvaluateGlobalTransform(const int64_t &boneID)
{
  atAssert(Contains(boneID), "The skeleton does not contain the specified bone");

  // Walk up the hierarchy and evaluating transforms from the top most parent
  atMat4D accumTransform = atMat4D::Identity();
  VisitParent(
    // Traversing up the tree
    [&accumTransform](int64_t nodeID, atHierarchy<atBone> *pTree) {
      atBone &bone = pTree->Get(nodeID);
      if (!bone.modified)
        accumTransform = bone.globalTransform;
      else
        accumTransform = bone.localTransform;
      return bone.modified;
    },
    boneID,

      // Traversing back down
    [&accumTransform](int64_t nodeID, atHierarchy<atBone> *pTree) {
      atBone &bone = pTree->Get(nodeID);
      bone.globalTransform = accumTransform * bone.localTransform;
      accumTransform = bone.globalTransform;
      bone.modified = false; // Reset the modified flag
    });

  return accumTransform;
}

atMat4D atSkeleton::GetLocalTransform(const int64_t &boneID) const
{
  atAssert(Contains(boneID), "The skeleton does not contain the specified bone");
  return Get(boneID).localTransform;
}

atMat4D atSkeleton::GetGlobalTransform(const int64_t &boneID) const
{
  atAssert(Contains(boneID), "The skeleton does not contain the specified bone");
  return Get(boneID).globalTransform;
}

atPose& atSkeleton::AddPose(const bool &applyCurrentTransforms, const atString &name)
{
  m_poses.emplace_back(this, name);
  if (applyCurrentTransforms)
    SetPose(m_poses.size() - 1);
  return m_poses.back();
}

void atSkeleton::SetPose(const int64_t &index)
{
  atPose &pose = m_poses[index];
  Visit([this, &pose](int64_t id, atHierarchy<atBone> *pTree) {
    pose.SetTransform(id, EvaluateGlobalTransform(id));
    return true;
  });
}

void atSkeleton::MoveToPose(const int64_t &index)
{
  atPose &pose = GetPose(index);
  Visit([this, &pose](int64_t id, atHierarchy<atBone> *pTree) {
    SetGlobalTransform(id, pose.GetTransform(id));
    return true;
  });
}

atSkeleton& atSkeleton::operator=(const atSkeleton &rhs)
{
  m_poses = rhs.m_poses;
  for (atPose &pose : m_poses)
    pose.m_pSkeleton = this;
  atHierarchy<atBone>::operator=(rhs);
  return *this;
}

atSkeleton& atSkeleton::operator=(atSkeleton &&rhs)
{
  m_poses = std::move(rhs.m_poses);
  for (atPose &pose : m_poses)
    pose.m_pSkeleton = this;
  atHierarchy<atBone>::operator=(std::move(rhs));
  return *this;
}

void atSkeleton::FlagModified(const int64_t &boneID)
{
  atAssert(Contains(boneID), "The skeleton does not contain the specified bone");
  // Recursively set the modified flags in the hierarchy
  Visit([](int64_t id, atHierarchy<atBone> *pTree) {
    atBone &bone = pTree->Get(id);
    if (bone.modified)
      return false;
    bone.modified = true;
    return true;
  }, boneID);
}
