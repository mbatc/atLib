#ifndef atThreading_h__
#define atThreading_h__

#include <thread>
#include <mutex>

class atScopeLock
{
public:
  atScopeLock(std::mutex &_mutex);
  ~atScopeLock();

private:
  std::mutex &m_mutex;
};

class atRecursiveScopeLock
{
public:
  atRecursiveScopeLock(std::recursive_mutex &_mutex);
  ~atRecursiveScopeLock();

private:
  std::recursive_mutex &m_mutex;
};

void atSleep(const int64_t milliseconds);

#endif // atThreading_h__
