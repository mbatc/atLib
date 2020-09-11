#ifndef atSerialize_h__
#define atSerialize_h__

#include "atObjectDescriptor.h"

// Built-ins
void atSerialize(atObjectDescriptor *pSerialized, const bool &src);
void atSerialize(atObjectDescriptor *pSerialized, const double &src);
void atSerialize(atObjectDescriptor *pSerialized, const float &src);
void atSerialize(atObjectDescriptor *pSerialized, const int8_t &src);
void atSerialize(atObjectDescriptor *pSerialized, const int16_t &src);
void atSerialize(atObjectDescriptor *pSerialized, const int32_t &src);
void atSerialize(atObjectDescriptor *pSerialized, const uint8_t &src);
void atSerialize(atObjectDescriptor *pSerialized, const int64_t &src);
void atSerialize(atObjectDescriptor *pSerialized, const uint16_t &src);
void atSerialize(atObjectDescriptor *pSerialized, const uint32_t &src);
void atSerialize(atObjectDescriptor *pSerialized, const uint64_t &src);
void atSerialize(atObjectDescriptor *pSerialized, const char *src);
void atSerialize(atObjectDescriptor *pSerialized, const atString &src);

void atDeserialize(const atObjectDescriptor &serialized, bool *pDst);
void atDeserialize(const atObjectDescriptor &serialized, double *pDst);
void atDeserialize(const atObjectDescriptor &serialized, float *pDst);
void atDeserialize(const atObjectDescriptor &serialized, int8_t *pDst);
void atDeserialize(const atObjectDescriptor &serialized, int16_t *pDst);
void atDeserialize(const atObjectDescriptor &serialized, int32_t *pDst);
void atDeserialize(const atObjectDescriptor &serialized, int64_t *pDst);
void atDeserialize(const atObjectDescriptor &serialized, uint8_t *pDst);
void atDeserialize(const atObjectDescriptor &serialized, uint16_t *pDst);
void atDeserialize(const atObjectDescriptor &serialized, uint32_t *pDst);
void atDeserialize(const atObjectDescriptor &serialized, uint64_t *pDst);
void atDeserialize(const atObjectDescriptor &serialized, atString *pDrc);


// Math types
template<typename T> void atSerialize(atObjectDescriptor *pSerialized, const atVector2<T> &src);
template<typename T> void atSerialize(atObjectDescriptor *pSerialized, const atVector3<T> &src);
template<typename T> void atSerialize(atObjectDescriptor *pSerialized, const atVector4<T> &src);
template<typename T> void atSerialize(atObjectDescriptor *pSerialized, const atMatrix4x4<T> &src);
template<typename T> void atSerialize(atObjectDescriptor *pSerialized, const atQuaternion<T> &src);

template<typename T> void atDeserialize(const atObjectDescriptor &serialized, atVector2<T> *pDst);
template<typename T> void atDeserialize(const atObjectDescriptor &serialized, atVector3<T> *pDst);
template<typename T> void atDeserialize(const atObjectDescriptor &serialized, atVector4<T> *pDst);
template<typename T> void atDeserialize(const atObjectDescriptor &serialized, atMatrix4x4<T> *pDst);
template<typename T> void atDeserialize(const atObjectDescriptor &serialized, atQuaternion<T> *pDst);


// Containers
template<typename T> void atSerialize(atObjectDescriptor *pSerialized, const T *pSrc, int64_t count);
template<typename T> void atSerialize(atObjectDescriptor *pSerialized, const atVector<T> &src);
template<typename Key, typename Value> void atSerialize(atObjectDescriptor *pSerialized, const atHashMap<Key, Value> &src);

template<typename T> void atDeserialize(const atObjectDescriptor &serialized, T *pDst, int64_t count);
template<typename T> void atDeserialize(const atObjectDescriptor &serialized, atVector<T> *pDst);
template<typename Key, typename Value> void atDeserialize(const atObjectDescriptor &serialized, atHashMap<Key, Value> *pDst);

#include "atSerialize.inl"
#endif // atSerialize_h__
