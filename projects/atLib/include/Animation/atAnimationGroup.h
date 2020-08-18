#ifndef atAnimationGroup_h__
#define atAnimationGroup_h__

#include "atPool.h"
#include "atString.h"
#include "atAnimation.h"

class atAnimationGroup
{
public:
  const atString& GetName() const;
  void SetName(const atString &name);

  // Add an animation to the group
  int64_t AddAnimation();
  
  // Get an animation in the group
  atAnimation* GetAnimation(const int64_t &animID);

  // Remove an animation from the group
  bool RemoveAnimation(const int64_t &animID);

protected:
  atString m_name;
  atPool<atAnimation> m_anims;
};

#endif // atAnimationGroup_h__
