
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

#ifndef atObject_h__
#define atObject_h__

#include <typeindex>
#include <type_traits>
#include "atHashMap.h"
#include "atString.h"

class atObject
{
public:
  atObject();
  atObject(const atObject &copy);
  atObject(atObject &&move);
  ~atObject();

  void Assign(const atObject &value);
  void Assign(atObject &&value);
  void SetMember(const atString &name, const atObject &value);
  void SetMember(const atString &name, atObject &&value);
  void Destroy();

  bool HasMember(const atString &name) const;

  atObject& GetMember(const atString &name);
  const atObject& GetMember(const atString &name) const;

  atObject& operator=(const atObject &rhs);
  atObject& operator=(atObject &&rhs);

  atObject& operator[](const atString &name);
  const atObject& operator[](const atString &name) const;

  template<typename T>
  bool Is() const;

  template<typename T>
  typename const std::enable_if<!std::is_void<T>::value, T>::type& As() const;

  template<typename T>
  typename std::enable_if<std::is_void<T>::value, T>::type As() const;

  template<typename T>
  T AsOr(const T &defVal) const;

  template<typename T>
  atObject& operator=(const T &val);

  template<typename T>
  atObject& operator=(T &&val);

  template<typename T>
  bool operator==(const T &val) const;

  template<typename T>
  bool operator!=(const T &val) const;

  template<typename T>
  atObject(const T &val);

  template<typename T>
  atObject(T &&val);

  template<typename T>
  void Assign(const T &value);

  template<typename T>
  void Assign(T &&value);

  template<typename T>
  void SetMember(const atString &name, const T &value);

  template<typename T>
  void SetMember(const atString &name, T &&value);

  template<typename T>
  T& GetMember(const atString &name);

  template<typename T>
  const T& GetMember(const atString &name) const;

  template<typename T> T GetMemberOr(const atString &name, const T &defVal) const;

  bool Empty() const;

  atString Typename() const;

protected:
  std::type_index m_typeInfo;
  atVector<uint8_t> m_data;
  atHashMap<atString, atObject> m_members;

  void MirrorType(const atObject &o);
  template<typename T> void SetType();

  void(*m_copyFunc) (atVector<uint8_t> *pMem, const void *) = nullptr;
  void(*m_moveFunc) (atVector<uint8_t> *pMem, void *) = nullptr;
  void(*m_destructFunc) (atVector<uint8_t> *pMem) = nullptr;
};

template<typename T> void __atObjectDestructFunc(atVector<uint8_t> *pMem);
template<typename T> void __atObjectCopyFunc(atVector<uint8_t> *pMem, const void *pData);
template<typename T> void __atObjectMoveFunc(atVector<uint8_t> *pMem, void *pData);

#include "atObject.inl"
#endif // atObject_h__
