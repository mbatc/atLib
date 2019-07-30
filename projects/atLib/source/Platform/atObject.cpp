
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
  m_typeInfo = copy.m_typeInfo;
  m_members = copy.m_members;
  m_data = copy.m_data;
}

atObject::atObject(atObject &&move)
  : atObject()
{
  Assign(move);
}

atObject::~atObject()
{
  if (m_destructFunc)
    m_destructFunc(m_data.data());
}

void atObject::Assign(const atObject &value)
{
  m_typeInfo = value.m_typeInfo;
  m_members = value.m_members;
  m_data = value.m_data;
}

void atObject::Assign(atObject &&value)
{
  m_typeInfo = std::move(value.m_typeInfo);
  m_members = std::move(value.m_members);
  m_data = std::move(value.m_data);
  m_destructFunc = value.m_destructFunc;
  value.m_destructFunc = nullptr;
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
