#ifndef atResourceManager_h__
#define atResourceManager_h__

#include "atSerialize.h"
#include "atResourceHandler.h"
#include "atObjectDescriptor.h"

class atResourceManager
{
public:
  template<typename T> atResourceHandle Request(const atObjectDescriptor &request);

  template<typename T> bool CanHandle();

  template<typename T> bool AddHandler();
  template<typename T> bool HasHandler();
  template<typename T> bool RemoveHandle();

protected:
  atVector<atResourceHandlerBase*> m_handlers;
  atVector<std::type_index> m_handlerType;
};

#include "atResourceManager.inl"
#endif // atResourceManager_h__

