
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
  if (m_destructFunc)
    m_destructFunc(m_data.data());
  m_typeInfo = typeid(T);
  m_data.resize(sizeof(T));
  T* pMem = (T*)m_data.data();
  new(pMem) T(value);
  m_destructFunc = atObjectDestructFunc<T>;
}

template<typename T>
inline void atObject::SetMember(const atString &name, const T &value)
{
  SetMember(name, atObject(value));
}

template<typename T>
inline void atObjectDestructFunc(void *pData)
{
  T* pObj = (T*)pData;
  pObj->~T();
}