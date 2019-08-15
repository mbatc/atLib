
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
  Assign(move);
}

atObject::~atObject() { Destroy(); }

void atObject::Assign(const atObject &value)
{
  if (m_typeInfo != value.m_typeInfo)
  {
    Destroy();
    m_typeInfo = value.m_typeInfo;

    // Get the helper functions
    m_destructFunc = value.m_destructFunc;
    m_copyFunc = value.m_copyFunc;
    m_moveFunc = value.m_moveFunc;
    m_copyConstructFunc = value.m_copyConstructFunc;
    m_moveConstructFunc = value.m_moveConstructFunc;

    // Copy construct into m_data
    m_copyConstructFunc(&m_data, value.m_data.data());
  }
  else
  { // Copy assign into m_data
    m_copyFunc(&m_data, value.m_data.data());
  }
  // Copy members
  m_members = value.m_members;
}

void atObject::Assign(atObject &&value)
{
  if (m_typeInfo != value.m_typeInfo)
  {
    // Destroy and get new type info
    Destroy();
    m_typeInfo = std::move(value.m_typeInfo);

    // Get the helper functions
    m_destructFunc = value.m_destructFunc;
    m_copyFunc = value.m_copyFunc;
    m_moveFunc = value.m_moveFunc;
    m_copyConstructFunc = value.m_copyConstructFunc;
    m_moveConstructFunc = value.m_moveConstructFunc;

    // Move construct into m_data
    m_moveConstructFunc(&m_data, value.m_data.data());
  }
  else
  {
    // Move assign into m_data
    m_moveFunc(&m_data, value.m_data.data());
  }

  // Move members
  m_members = std::move(value.m_members);
}

void atObject::SetMember(const atString &name, const atObject &value)
{
  if (!HasMember(name))
    m_members.Add(name);
  m_members[name] = value;
}

void atObject::SetMember(const atString &name, atObject &&value)
{
  if (!HasMember(name))
    m_members.Add(name);
  m_members[name] = std::move(value);
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

const atObject& atObject::operator=(const atObject &rhs)
{
  Assign(rhs);
  return *this;
}

const atObject& atObject::operator=(atObject &&rhs)
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

void atObject::Destroy()
{
  if (m_destructFunc)
    m_destructFunc(m_data.data());
  m_data.clear();
  m_copyFunc = 0;
  m_moveFunc = 0;
  m_destructFunc = 0;
  m_moveConstructFunc = 0;
  m_copyConstructFunc = 0;
}
