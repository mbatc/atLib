#ifndef atSerialize_h__
#define atSerialize_h__

#include "atSerializable.h"

template<typename T> void atSerialize(atSerializable *pSerialized, T *pSource, int64_t count);
template<typename T> void atDeserialize(const atSerializable &serialized, T *pDst, int64_t count);

#endif // atSerialize_h__
