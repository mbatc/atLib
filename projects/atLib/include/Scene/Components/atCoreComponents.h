#ifndef atCoreComponents_h__
#define atCoreComponents_h__

#include "atString.h"

enum atCoreComponentType
{
  atCCT_Unknown,
  atCCT_Transform,
  atCCT_Camera,
  atCCT_Mesh,
  atCCT_Control,
  atCCT_Script,
  atCCT_Count
};

extern atString atCoreComponentType_Name[atCCT_Count];

atCoreComponentType atCoreComponentType_FromString(const atString &name);

#endif // atCoreComponents_h__
