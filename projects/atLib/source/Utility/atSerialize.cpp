#include "atSerialize.h"

void atSerialize(atObjectDescriptor *pSerialized, const bool &src) { pSerialized->Set(src); }
void atSerialize(atObjectDescriptor *pSerialized, const double &src) { pSerialized->Set(src); }
void atSerialize(atObjectDescriptor *pSerialized, const float &src) { pSerialized->Set((double)src); }
void atSerialize(atObjectDescriptor *pSerialized, const int8_t &src) { pSerialized->Set((int64_t)src); }
void atSerialize(atObjectDescriptor *pSerialized, const int16_t &src) { pSerialized->Set((int64_t)src); }
void atSerialize(atObjectDescriptor *pSerialized, const int32_t &src) { pSerialized->Set((int64_t)src); }
void atSerialize(atObjectDescriptor *pSerialized, const uint8_t &src) { pSerialized->Set((int64_t)src); }
void atSerialize(atObjectDescriptor *pSerialized, const int64_t &src) { pSerialized->Set(src); }
void atSerialize(atObjectDescriptor *pSerialized, const uint16_t &src) { pSerialized->Set((int64_t)src); }
void atSerialize(atObjectDescriptor *pSerialized, const uint32_t &src) { pSerialized->Set((int64_t)src); }
void atSerialize(atObjectDescriptor *pSerialized, const uint64_t &src) { pSerialized->Set((int64_t)src); }
void atSerialize(atObjectDescriptor *pSerialized, const char *src) { pSerialized->Set(atString(src)); }
void atSerialize(atObjectDescriptor *pSerialized, const atString &src) { pSerialized->Set(src); }

void atDeserialize(const atObjectDescriptor &serialized, bool *pDst) { *pDst = serialized.AsBool(); }
void atDeserialize(const atObjectDescriptor &serialized, double *pDst) { *pDst = serialized.AsFloat(); }
void atDeserialize(const atObjectDescriptor &serialized, float *pDst) { *pDst = (float)serialized.AsFloat(); }
void atDeserialize(const atObjectDescriptor &serialized, int8_t *pDst) { *pDst = (int8_t)serialized.AsInt(); }
void atDeserialize(const atObjectDescriptor &serialized, int16_t *pDst) { *pDst = (int16_t)serialized.AsInt(); }
void atDeserialize(const atObjectDescriptor &serialized, int32_t *pDst) { *pDst = (int32_t)serialized.AsInt(); }
void atDeserialize(const atObjectDescriptor &serialized, int64_t *pDst) { *pDst = (int64_t)serialized.AsInt(); }
void atDeserialize(const atObjectDescriptor &serialized, uint8_t *pDst) { *pDst = (uint8_t)serialized.AsInt(); }
void atDeserialize(const atObjectDescriptor &serialized, uint16_t *pDst) { *pDst = (uint16_t)serialized.AsInt(); }
void atDeserialize(const atObjectDescriptor &serialized, uint32_t *pDst) { *pDst = (uint32_t)serialized.AsInt(); }
void atDeserialize(const atObjectDescriptor &serialized, uint64_t *pDst) { *pDst = (uint64_t)serialized.AsInt(); }
void atDeserialize(const atObjectDescriptor &serialized, atString *pDst) { *pDst = serialized.AsString(); }
