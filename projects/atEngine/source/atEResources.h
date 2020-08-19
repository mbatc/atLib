#ifndef atEResources_h__
#define atEResources_h__

#include "atResourceManager.h"

class atEResources
{
public:
  static atEResources* Instance();
  static void Destroy();

  atResourceManager& GetManager();

protected:
  atResourceManager m_manager;
};

#endif // atEResources_h__
