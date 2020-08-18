#ifndef atBone_h__
#define atBone_h__

#include "atString.h"

class atBone
{
public:
  atString name;

  atMat4D localTransform = atMat4D::Identity();  // The bones current local transform
  atMat4D globalTransform = atMat4D::Identity(); // The bones evaluated global transform

  bool modified = false; // Counter used to determine if the bones transform has already been evaluated
};

#endif // atBone_h__
