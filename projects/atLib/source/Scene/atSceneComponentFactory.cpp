#include "atSceneComponentFactory.h"

std::mutex atSceneComponentFactory::m_lock;
atHashMap<atString, int64_t> atSceneComponentFactory::m_nameToID;
atVector<atSceneComponentFactory::ComponentInterface> atSceneComponentFactory::m_registry;

atSceneComponent* atSceneComponentFactory::Create(const atString &stringID)
{
  AllocatorT allocator = nullptr;
  {
    atScopeLock lock(m_lock);
    int64_t *pID = m_nameToID.TryGet(stringID);
    if (!pID)
      return nullptr;
    allocator = m_registry[*pID].allocator;
  }

  return allocator ? allocator() : nullptr;
}

int64_t atSceneComponentFactory::NextComponentID()
{
  atScopeLock lock(m_lock);
  static int64_t _nextComponentID = 0;
  return _nextComponentID++;
}
