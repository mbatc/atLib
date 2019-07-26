#include "atThreading.h"

atScopeLock::atScopeLock(std::mutex &_mutex) : m_mutex(_mutex) { m_mutex.lock(); }
atScopeLock::~atScopeLock() { m_mutex.unlock(); }
atRecursiveScopeLock::atRecursiveScopeLock(std::recursive_mutex &_mutex) : m_mutex(_mutex) { m_mutex.lock(); }
atRecursiveScopeLock::~atRecursiveScopeLock() { m_mutex.unlock(); }

void atSleep(const int64_t milliseconds)
{
  if (milliseconds == 0)
    atYield();
  else
    std::this_thread::sleep_for(std::chrono::duration<int64_t, std::milli>(milliseconds));
}

void atYield() { std::this_thread::yield(); }

atConditionalScopeLock::atConditionalScopeLock(std::mutex &_mutex, const bool &shouldLock)
  : m_mutex(_mutex)
  , m_isLocked(shouldLock)
{
  if (shouldLock)
    m_mutex.lock();
}

atConditionalScopeLock::~atConditionalScopeLock()
{
  if (m_isLocked)
    m_mutex.unlock();
}

atConditionalRecursiveScopeLock::atConditionalRecursiveScopeLock(std::recursive_mutex &_mutex, const bool &shouldLock)
  : m_mutex(_mutex)
  , m_isLocked(shouldLock)
{
  if (shouldLock)
    m_mutex.lock();
}

atConditionalRecursiveScopeLock::~atConditionalRecursiveScopeLock()
{
  if (m_isLocked)
    m_mutex.unlock();
}

atThreadedObject::atThreadedObject(void *pUserData)
  : m_isPaused(true)
  , m_pUserData(pUserData)
  , m_shouldPause(true)
  , m_isRunning(true)
  , m_shouldStop(false)
  , m_nextSleepTime(0)
  , m_threadID(0)
  , m_thread(&atThreadedObject::Run, this)
{
  m_threadID = (uint64_t)std::hash<std::thread::id>()(m_thread.get_id());
}

atThreadedObject::~atThreadedObject()
{
  Stop();
  while (IsRunning());
  m_thread.join();
}

void atThreadedObject::Start()
{
  m_shouldPause = false;
  m_shouldStop = false;
}

void atThreadedObject::Sleep(const int64_t duration)
{
  atScopeLock lock(m_writeLock);
  m_nextSleepTime += duration;
}

void atThreadedObject::Run(atThreadedObject *pBase)
{
  while (pBase->IsRunning())
  {
    if (!pBase->IsPaused())
    {
      switch (pBase->Execute(pBase->m_pUserData))
      {
      case CO_Pause: pBase->Pause(); break;
      case CO_Sleep: pBase->Sleep(); break;
      case CO_Stop: pBase->Stop(); break;
      case CO_Continue: default: break;
      }
    }
    else
    {
      pBase->Sleep();
    }

    {
      atScopeLock lock(pBase->m_writeLock);
      pBase->m_isPaused = pBase->m_shouldPause;
      pBase->m_isRunning = !pBase->m_shouldStop;
    }
  }
}

void atThreadedObject::Sleep()
{
  m_isSleeping = true;
  atSleep(SleepTime() + m_nextSleepTime);
  
  m_writeLock.lock();
  m_nextSleepTime = 0; // Lock here as two threads could write to this variable
  m_writeLock.unlock();

  m_isSleeping = false;
}

void atThreadedObject::Pause() { m_shouldPause = true; }
void atThreadedObject::Stop() { m_shouldStop = true; }

bool atThreadedObject::IsRunning()
{
  atScopeLock lock(m_writeLock);
  return m_isRunning;
}

bool atThreadedObject::IsPaused()
{
  atScopeLock lock(m_writeLock);
  return m_isPaused;
}
bool atThreadedObject::IsSleeping()
{
  atScopeLock lock(m_writeLock);
  return m_isSleeping;
}

uint64_t atThreadedObject::ID() const { return m_threadID; }
std::thread::id atThreadedObject::ThreadID() const { return m_thread.get_id(); }
