#ifndef atResourceHandler_h__
#define atResourceHandler_h__

#include "atResourceHandle.h"

class atResourceHandlerBase
{
public:
  atResourceHandlerBase(const atString &resourceTypeID)
    : m_resourceTypeID(resourceTypeID)
  {}

  template<typename T> bool Handles() const { return Handles(typeid(T)); }

  atResourceHandle Request(const atObjectDescriptor &request)
  {
    atResourceGUID guid(m_resourceTypeID, request);
    atResourceHandle newHandle;
    {
      atScopeLock lock(m_resourceLock);
      atResourceHandle *pHandle = m_resources.TryGet(guid);
      if (pHandle)
        return *pHandle;

      newHandle = atResourceHandle(guid, this);
      m_resources.Add(guid, newHandle);
    }

    atObject resource = _Load(request);
    if (resource.Empty())
    { // Load failed
      atScopeLock lock(m_resourceLock); // Erase this resource
      m_resources.Remove(guid);
      newHandle.m_pInstance->refLock.lock();
      newHandle.m_pInstance->loaded = false;
      newHandle.m_pInstance->loadFailed = true;
      newHandle.m_pInstance->refLock.unlock();
      return newHandle;
    }

    newHandle.m_pInstance->refLock.lock();
    newHandle.m_pInstance->resource = std::move(resource);
    newHandle.m_pInstance->loaded = true;
    newHandle.m_pInstance->loadFailed = false;
    newHandle.m_pInstance->refLock.unlock();
    return newHandle;
  }

protected:
  virtual bool Handles(const std::type_index &typeInfo) = 0;
  virtual atObject _Load(const atObjectDescriptor &request) = 0;

  atHashMap<atResourceGUID, atResourceHandle> m_resources;
  std::mutex m_resourceLock;
  const atString m_resourceTypeID;
};

template<typename T> class atResourceHandler : public atResourceHandlerBase
{
public:
  virtual bool Load(const atObjectDescriptor &request, T *pResource) = 0;

protected:
  atObject _Load(const atObjectDescriptor &request) override final
  {
    uint8_t buffer[sizeof(T)] = { 0 };
    if (Load(request, (T *)buffer))
      return std::move(buffer);
    return atObject();
  }

  virtual bool Handles(const std::type_index &typeInfo) { return typeInfo == typeid(T); }
};

#endif // atResourceHandler_h__
