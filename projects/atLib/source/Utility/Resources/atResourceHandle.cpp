#include "atResourceHandle.h"

atResourceHandle::atResourceHandle(atResourceHandle &&o) { *this = std::move(o); }
atResourceHandle::atResourceHandle(const atResourceHandle &o) { *this = o; }
atResourceHandle::atResourceHandle() : m_pInstance(nullptr) {}

atResourceHandle::atResourceHandle(const atResourceGUID &guid, atResourceHandlerBase *pHandler)
{
  m_pInstance = atNew<Instance>();
  m_pInstance->pHandler = pHandler;
  m_pInstance->guid = guid;
  m_pInstance->refCount = 1;
}

atResourceHandle::~atResourceHandle() { ReleaseRef(); }

atResourceHandle &atResourceHandle::operator=(const atResourceHandle &o)
{
  TakeRef(o.m_pInstance);
  return *this;
}

atResourceHandle &atResourceHandle::operator=(atResourceHandle &&o)
{
  TakeRef(o.m_pInstance);
  o.ReleaseRef();
  return *this;
}

bool atResourceHandle::HasResource() const { return m_pInstance && !m_pInstance->resource.Empty(); }
bool atResourceHandle::IsLoaded() const { return m_pInstance && m_pInstance->loaded; }
bool atResourceHandle::LoadFailed() const { return m_pInstance && m_pInstance->loadFailed; }

void atResourceHandle::TakeRef(Instance *pInstance)
{
  if (pInstance == m_pInstance)
    return;

  ReleaseRef();
  m_pInstance = pInstance;
  if (!m_pInstance)
    return;
  ++m_pInstance->refCount;
}

void atResourceHandle::ReleaseRef()
{
  if (!m_pInstance)
    return;

  bool deleteInstance = false;

  Instance *pInstance = m_pInstance;
  m_pInstance = nullptr;

  pInstance->refLock.lock();
  if (--pInstance->refCount == 0)
    deleteInstance = true; // No more references, unlock and then delete the resource
  if (pInstance->refCount == 1 && pInstance->pHandler)
  { // Remove reference from the resource handler
  }

  pInstance->refLock.unlock();

  if (deleteInstance)
    atDelete(pInstance);
}
