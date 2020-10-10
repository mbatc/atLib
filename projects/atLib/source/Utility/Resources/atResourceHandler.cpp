#include "atResourceHandler.h"

atResourceHandlerBase::atResourceHandlerBase(const atString &resourceTypeID) : m_resourceTypeID(resourceTypeID) {}

atResourceHandle atResourceHandlerBase::Request(const atObjectDescriptor &request)
{
  atResourceGUID guid(m_resourceTypeID, request);
  atResourceHandle::Instance *pNewHandle = nullptr;
  {
    atScopeLock lock(m_resourceLock);
    atResourceHandle::Instance **ppHandle = m_resources.TryGet(guid);
    if (ppHandle)
      return *ppHandle;

    pNewHandle = atNew(atResourceHandle::Instance);
    pNewHandle->pHandler = this;
    pNewHandle->guid = guid;
    pNewHandle->refCount = 0;
    m_resources.Add(guid, pNewHandle);
  }

  atObject resource = _Load(request);
  if (resource.Empty())
  { // Load failed
    atScopeLock lock(m_resourceLock); // Erase this resource
    m_resources.Remove(guid);
    pNewHandle->refLock.lock();
    pNewHandle->loaded = false;
    pNewHandle->loadFailed = true;
    pNewHandle->refLock.unlock();
    return pNewHandle;
  }

  pNewHandle->refLock.lock();
  pNewHandle->resource = std::move(resource);
  pNewHandle->loaded = true;
  pNewHandle->loadFailed = false;
  pNewHandle->refLock.unlock();
  return pNewHandle;
}

atResourceManager* atResourceHandlerBase::GetResourceManager() const { return m_pManager; }

bool atResourceHandlerBase::Remove(const atResourceGUID &guid)
{
  atResourceHandle::Instance **ppResource = m_resources.TryGet(guid);
  if (ppResource)
  {
    atResourceHandle::Instance *pResource = *ppResource;
    m_resources.Remove(guid);
    atDelete(pResource);
  }

  return true;
}
