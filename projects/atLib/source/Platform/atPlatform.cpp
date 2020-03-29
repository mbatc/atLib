#include "atPlatform.h"


atPlatform_OS atPlatform::GetOS()
{
#ifdef atPLATFORM_WIN32
  return atPlatform_OS_Win32;
#elif atPLATFORM_LINUX
  return atPlatform_OS_Linux;
#else
  return atPlatform_OS_None;
#endif
} 