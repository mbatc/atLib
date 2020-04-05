#include <utility>

template<typename T> inline T atMin(const T &a) { return a; }
template<typename T> inline T atMax(const T &a) { return a; }

template<typename T, typename T2, typename... Args> inline T atMin(const T &first, const T2 &second, Args ...args)
{
  T argMin = (T)atMin(second, std::forward<Args>(args)...);
  return (argMin < first) ? argMin : first;
}

template<typename T, typename T2, typename... Args> inline T atMax(const T &first, const T2 &second, Args ...args)
{
  T argMin = (T)atMax(second, std::forward<Args>(args)...);
  return (argMin > first) ? argMin : first;
}
