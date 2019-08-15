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
inline const T& atObject::As() const
{
  return *(T*)m_data.data();
}

template<typename T>
inline const atObject& atObject::operator=(const T &val)
{
  Assign(val);
  return *this;
}

template<typename T>
inline const atObject& atObject::operator=(T &&val)
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
inline void atObject::Assign(const T &value)
{
  if (m_typeInfo != value.m_typeInfo)
  {
    Destroy();
    SetType<T>();
    // Copy construct into m_data
    m_copyConstructFunc(&m_data, &value);
  }
  else
  {
    // Copy assign into m_data
    m_copyFunc(&m_data, &value);
  }

  // Copy members
  m_members = value.m_members;
}

template<typename T>
inline void atObject::Assign(T &&value)
{
  if (m_typeInfo != value.m_typeInfo)
  {
    Destroy();
    SetType<T>();
    // Move construct into m_data
    m_moveConstructFunc(&m_data, &value);
  }
  else
  {
    // Move assign into m_data
    m_moveFunc(&m_data, &value);
  }

  // Copy members
  m_members = value.m_members;
}

template<typename T>
inline void atObject::SetMember(const atString &name, const T &value)
{
  SetMember(name, atObject(value));
}

template<typename T> void atObject::SetType()
{
  m_typeInfo = typeid(T);
  m_data.resize(sizeof(T));
  m_moveFunc = &__atObjectMoveFunc<T>;
  m_copyFunc = &__atObjectCopyFunc<T>;
  m_destructFunc = &__atObjectDestructFunc<T>;
  m_copyConstructFunc = &__atObjectCopyConstructFunc<T>;
  m_moveConstructFunc = &__atObjectMoveConstructFunc<T>;
}

template<typename T>
inline void __atObjectDestructFunc(void *pData)
{
  atDestruct((T*)pData);
}

template<typename T>
inline void __atObjectMoveFunc(atVector<uint8_t> *pDst, void *pSrc)
{
  pDst->resize(sizeof(T));
  *(T*)pDst->data() = std::move(*(T*)pSrc);
}

template<typename T>
inline void __atObjectCopyFunc(atVector<uint8_t> *pDst, const void *pSrc)
{
  pDst->resize(sizeof(T));
  *(T*)pDst->data() = *(T*)pSrc;
}

template<typename T>
inline void __atObjectMoveConstructFunc(atVector<uint8_t> *pDst, void *pSrc)
{
  pDst->resize(sizeof(T));
  atConstruct(pDst->data(), std::move(*(T*)pSrc));
}

template<typename T>
inline void __atObjectCopyConstructFunc(atVector<uint8_t> *pDst, const void *pSrc)
{
  pDst->resize(sizeof(T));
  atConstruct(pDst->data(), *(T*)pSrc);
}
