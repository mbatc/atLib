
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

#include "atString.h"
#include "atHashMap.h"
#include <typeindex>

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

  bool HasMember(const atString &name) const;

  atObject& GetMember(const atString &name);
  const atObject& GetMember(const atString &name) const;

  const atObject& operator=(const atObject &rhs);
  const atObject& operator=(atObject &&rhs);

  atObject& operator[](const atString &name);
  const atObject& operator[](const atString &name) const;

  template<typename T>
  bool Is() const;

  template<typename T>
  const T& As() const;

  template<typename T>
  const atObject& operator=(const T &val);

  template<typename T>
  const atObject& operator=(T &&val);

  template<typename T>
  bool operator==(const T &val) const;
  
  template<typename T>
  bool operator!=(const T &val) const;

  template<typename T>
  atObject(const T &val);

  template<typename T>
  void Assign(const T &value);

  // template<typename T>
  // void Assign(T &&value);

  template<typename T>
  void SetMember(const atString &name, const T &value);

  template<typename T>
  T& GetMember(const atString &name);

  template<typename T>
  const T& GetMember(const atString &name) const;

protected:
  std::type_index m_typeInfo;
  atVector<uint8_t> m_data;
  atHashMap<atString, atObject> m_members;

  void (*m_destructFunc) (void*);
};

template<typename T> void atObjectDestructFunc(void *pData);

#include "atObject.inl"
#endif // atObject_h__
