#include "atResourceHandle.h"
#include "atResourceHandler.h"

atResourceHandle::atResourceHandle(atResourceHandle &&o) { *this = std::move(o); }
atResourceHandle::atResourceHandle(const atResourceHandle &o) { *this = o; }
atResourceHandle::atResourceHandle() : atResourceHandle(nullptr) {}
atResourceHandle::atResourceHandle(Instance *pInstance) { TakeRef(pInstance); }

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

  Instance *pInstance = m_pInstance;
  m_pInstance = nullptr;

  pInstance->refLock.lock();
  bool deleteInstance = --pInstance->refCount == 0;
  pInstance->refLock.unlock();

  if (deleteInstance)
    pInstance->pHandler->Remove(pInstance->guid);
}
