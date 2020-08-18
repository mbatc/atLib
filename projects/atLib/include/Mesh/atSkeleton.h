#ifndef atSkeleton_h__
#define atSkeleton_h__

#include "atBone.h"
#include "atPose.h"
#include "atHierarchy.h"

// Skeleton is a hierarchy of bones and a collection
// of bind poses

class atSkeleton : public atHierarchy<atBone>
{
public:
  // Get the number of bind poses for this skeleton
  int64_t GetPoseCount() const;

  // Get a bind pose by index
  atPose& GetPose(const int64_t &poseIndex);
  const atPose& GetPose(const int64_t &poseIndex) const;

  // Remove a pose by index
  void RemovePose(const int64_t &index);

  // Set a bones local transform
  void SetTransform(const int64_t &boneID, const atMat4D &transform);

  // Set a bones global transform
  void SetGlobalTransform(const int64_t &boneID, const atMat4D &transform);

  // Evaluate the global transform for the specified bone
  atMat4D EvaluateGlobalTransform(const int64_t &boneID = atHierarchy_RootNodeID);

  // Get the currently stored local transform for the specified bone
  atMat4D GetLocalTransform(const int64_t &boneID) const;

  // Get the currently stored global transform. EvaluateGlobalTransform should
  // be called before using this function to ensure the transform is up to date.
  atMat4D GetGlobalTransform(const int64_t &boneID) const;
  
  // Add a pose using the current position of the bones
  atPose& AddPose(const bool &applyCurrentTransforms = true, const atString &name = "new pose");

  // Apply the current position of the bones to a pose
  void SetPose(const int64_t &index);

  // Move to a pose
  void MoveToPose(const int64_t &index);

  atSkeleton& operator=(const atSkeleton &rhs);
  atSkeleton& operator=(atSkeleton &&rhs);

protected:
  void FlagModified(const int64_t &boneID);

  atVector<atPose> m_poses;
};

#endif // atSkeleton_h__
