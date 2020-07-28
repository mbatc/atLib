#include "atObject.h"

// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

template<typename T>
inline bool atObject::Is() const
{
  return m_typeInfo == typeid(T);
}

template<typename T>
inline typename const std::enable_if<!std::is_void<T>::value, T>::type& atObject::As() const
{
  return *(T*)m_data.data();
}

template<typename T>
inline typename std::enable_if<std::is_void<T>::value, T>::type atObject::As() const {}

template<typename T>
inline T atObject::AsOr(const T &defVal) const
{
  if (!Is<T>())
    return defVal;
  return As<T>();
}

template<typename T>
inline atObject& atObject::operator=(const T &val)
{
  Assign(val);
  return *this;
}

template<typename T>
inline atObject& atObject::operator=(T &&val)
{
  Assign(val);
  return *this;
}

template<typename T>
inline bool atObject::operator==(const T &val) const
{
  return Is<T>() && (As<T>() == val);
}

template<typename T>
inline bool atObject::operator!=(const T &val) const
{
  return !(*this == val);
}

template<typename T>
inline atObject::atObject(const T &val)
  : atObject()
{
  Assign(val);
}

template<typename T>
inline atObject::atObject(T &&val)
  : atObject()
{
  Assign(std::forward<T>(val));
}

template<typename T>
inline void atObject::Assign(const T &value)
{
  SetType<T>();
  m_copyFunc(&m_data, &value);
}

template<typename T>
inline void atObject::Assign(T &&value)
{
  SetType<T>();
  m_moveFunc(&m_data, &value);
}

template<typename T>
inline void atObject::SetMember(const atString &name, const T &value)
{
  SetMember(name, atObject(value));
}

template<typename T>
inline void atObject::SetMember(const atString &name, T &&value)
{
  SetMember(name, atObject(std::move(value)));
}

inline void atObject::SetType(const atObject &obj)
{
  if (obj.m_typeInfo == m_typeInfo)
    return;

  Destroy();
  m_typeInfo = obj.m_typeInfo;
  m_moveFunc = obj.m_moveFunc;
  m_copyFunc = obj.m_copyFunc;
  m_destructFunc = obj.m_destructFunc;
  m_data.reserve(obj.m_data.size());
}

template<typename T>
inline void __atObjectDestructFunc(atVector<uint8_t> *pMem)
{
  T* pObj = (T*)pMem->data();
  pObj->~T();
  pMem->clear();
}

template<typename T>
inline void __atObjectCopyFunc(atVector<uint8_t> *pMem, const void *pData)
{
  const T* pSrc = (const T*)pData;
  T* pDst = (T*)pMem->data();
  new (pDst)T(*pSrc);
}

template<typename T>
inline void __atObjectMoveFunc(atVector<uint8_t> *pMem, void *pData)
{
  T* pSrc = (T*)pData;
  T* pDst = (T*)pMem->data();
  new (pDst)T(std::move(*pSrc));
}

template<typename T>
T atObject::GetMemberOr(const atString &name, const T &defVal) const
{
  if (!HasMember(name))
    return defVal;
  return GetMember(name).AsOr<T>(defVal);
}

template<typename T>
inline void atObjectWriteFunc(atWriteStream *pStream, atVector<uint8_t> *pData) { return atStreamWrite(pStream, (T*)pData->data(), 1); }

template<typename T>
inline void atObjectReadFunc(atReadStream *pStream, atVector<uint8_t> *pData) { return atStreamRead(pStream, (T*)pData->data(), 1); }
template<typename T>
inline void atObject::SetType()
{
  if (Is<T>())
    return;

  Destroy();
  m_destructFunc = __atObjectDestructFunc<std::decay_t<T>>;
  m_copyFunc = __atObjectCopyFunc<std::decay_t<T>>;
  m_moveFunc = __atObjectMoveFunc<std::decay_t<T>>;
  m_typeInfo = typeid(T);
  m_data.resize(sizeof(T));
}
