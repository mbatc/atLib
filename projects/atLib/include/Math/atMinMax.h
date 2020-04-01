#ifndef atMinMax_h__
#define atMinMax_h__

template<typename T> inline T atMin(const T &a);
template<typename T> inline T atMax(const T &a);
template<typename T, typename T2, typename... Args> inline T atMin(const T &first, const T2 &second, Args ...args);
template<typename T, typename T2, typename... Args> inline T atMax(const T &first, const T2 &second, Args ...args);

#ifdef atPLATFORM_WIN32
#include <minmax.h>
#elif atPLATFORM_LINUX
#endif

#include "atMinMax.inl"

#endif // atMinMax_h__
