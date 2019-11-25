#include "atCoreComponents.h"

atString atCoreComponentType_Name[atCCT_Count] =
{
  "scenecore_unknown",
  "scenecore_transform",
  "scenecore_camera",
  "scenecore_mesh",
  "scenecore_control",
  "scenecore_script"
};

atCoreComponentType atCoreComponentType_FromString(const atString &name)
{
  atString nameLwr = name.to_lower();
  for (int64_t i = 0; i < atArraySize(atCoreComponentType_Name); ++i)
    if (atCoreComponentType_Name[i] == nameLwr)
      return atCoreComponentType(i);
  return atCCT_Unknown;
}
