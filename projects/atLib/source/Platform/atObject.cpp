
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
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

#include "atObject.h"

atObject::atObject()
  : m_typeInfo(typeid(void))
  , m_destructFunc(nullptr)
{}

atObject::atObject(const atObject &copy)
  : atObject()
{
  Assign(copy);
}

atObject::atObject(atObject &&move)
  : atObject()
{
  Assign(std::move(move));
}

atObject::~atObject() { Destroy(); }

void atObject::Destroy()
{
  Destroy();
}

void atObject::Assign(const atObject &value)
{
  SetType(value);

  // Call type specific copy assign
  if (m_copyFunc)
    m_copyFunc(&m_data, value.m_data.data());

  // Copy members
  m_members = value.m_members;
}

void atObject::Assign(atObject &&value)
{
  SetType(value);

  // Call type specific move assign
  if (m_moveFunc)
    m_moveFunc(&m_data, value.m_data.data());

  // Move members
  m_members = std::move(value.m_members);
}

void atObject::SetMember(const atString &name, const atObject &value)
{
  m_members[name] = value;
}

void atObject::SetMember(const atString &name, atObject &&value)
{
  m_members[name] = std::move(value);
}

void atObject::Destroy()
{
  if (m_destructFunc)
    m_destructFunc(&m_data);
  m_destructFunc = nullptr;
  m_copyFunc = nullptr;
  m_moveFunc = nullptr;
  m_typeInfo = typeid(void);
}

bool atObject::HasMember(const atString &name) const
{
  return m_members.Contains(name);
}

atObject& atObject::GetMember(const atString &name)
{
  return m_members[name];
}

const atObject& atObject::GetMember(const atString &name) const
{
  return m_members[name];
}

atObject& atObject::operator=(const atObject &rhs)
{
  Assign(rhs);
  return *this;
}

atObject& atObject::operator=(atObject &&rhs)
{
  Assign(std::move(rhs));
  return *this;
}

atObject& atObject::operator[](const atString &name)
{
  return GetMember(name);
}

const atObject& atObject::operator[](const atString &name) const
{
  return GetMember(name);
}

bool atObject::Empty() const { return m_data.size() == 0 && m_members.Size() == 0 && m_typeInfo == typeid(void); }

atString atObject::Typename() const { return m_typeInfo.name(); }


int64_t atStreamRead(atReadStream * pStream, atObject * pData, const int64_t count)
{
  return int64_t();
}

int64_t atStreamWrite(atWriteStream * pStream, atObject * pData, const int64_t count)
{
  return int64_t();
}
