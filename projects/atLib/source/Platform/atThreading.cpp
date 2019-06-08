#include "atThreading.h"
#include <windows.h>

atScopeLock::atScopeLock(std::mutex &_mutex) : m_mutex(_mutex) { m_mutex.lock(); }
atScopeLock::~atScopeLock() { m_mutex.unlock(); }
atRecursiveScopeLock::atRecursiveScopeLock(std::recursive_mutex &_mutex) : m_mutex(_mutex) { m_mutex.lock(); }
atRecursiveScopeLock::~atRecursiveScopeLock() { m_mutex.unlock(); }

void atSleep(const int64_t milliseconds)
{
  Sleep((DWORD)milliseconds);
}
