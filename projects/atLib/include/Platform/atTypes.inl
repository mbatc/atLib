
template<typename T1, typename T2> bool __atTypeCast(T1 *pDst, const T2 *pSrc, const int64_t &count)
{
  for (int64_t i = 0; i < count; ++i)
    pDst[i] = (T1)pSrc[i];
  return true;
}

template<typename T> bool __atTypeCast(T *pDst, const T *pSrc, const int64_t &count)
{
  memcpy(pDst, pSrc, count * sizeof(T));
  return true;
}

template<typename T> bool __atTypeCast(void *pDst, const T *pSrc, const atType &dstType, const int64_t &count)
{
  switch (dstType)
  {
  case atType_Float16: return false;
  case atType_Float32: return __atTypeCast((float*)pDst, pSrc, count);
  case atType_Float64: return __atTypeCast((double*)pDst, pSrc, count);
  case atType_Int8: return __atTypeCast((int8_t*)pDst, pSrc, count);
  case atType_Int16: return __atTypeCast((int16_t*)pDst, pSrc, count);
  case atType_Int32: return __atTypeCast((int32_t*)pDst, pSrc, count);
  case atType_Int64: return __atTypeCast((int64_t*)pDst, pSrc, count);
  case atType_Uint8: return __atTypeCast((uint8_t*)pDst, pSrc, count);
  case atType_Uint16: return __atTypeCast((uint16_t*)pDst, pSrc, count);
  case atType_Uint32: return __atTypeCast((uint32_t*)pDst, pSrc, count);
  case atType_Uint64: return __atTypeCast((uint64_t*)pDst, pSrc, count);
  }
  return false;
}
