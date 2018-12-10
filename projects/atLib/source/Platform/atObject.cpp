#include "atObject.h"

const atObject& atObject::operator=(const atObject &copy)
{
  Assign(copy);
  return *this;
}

const atObject& atObject::operator=(atObject &&move)
{
  m_data = move.m_data;
  m_desc = move.m_desc;
  m_members = move.m_members;
  move.m_desc = atTypeDesc(atType_Unknown, 1);
  return move;
}

bool atObject::operator==(const atObject &value) const
{
  if (memcmp(&m_desc, &value.m_desc, sizeof(atTypeDesc)) != 0) return false;
  if (value.m_data.size() != m_data.size()) return false;
  if (memcmp(m_data.data(), value.m_data.data(), (size_t)value.m_data.size()) != 0) return false;
  for (auto &kvp : value.m_members)
  {
    const atObject *pObj = m_members.TryGet(kvp.m_key);
    if (!pObj || *pObj != kvp.m_val) return false;
  }
  return true;
}

atObject::atObject(atObject &&move) : atObject()
{
  m_desc = move.m_desc;
  m_data = move.m_data;
  m_members = move.m_members;
  move = atObject();
}

atObject::atObject(const atObject &copy) : atObject()
{
  m_desc = copy.m_desc;
  m_data = copy.m_data;
  m_members = copy.m_members;
}

void atObject::Assign(const atObject &value)
{
  m_data = value.m_data;
  m_desc = value.m_desc;
  m_members = value.m_members;
}

bool atObject::AddMember(const atString &name, const atObject &value)
{
  if (m_members.Contains(name))
    return false;
  m_members.Add(name, value);
  return true;
}

atObject::atObject() : m_desc(atType_Unknown, 1) {}
atObject::~atObject() {}
atObject& atObject::Get(const atString &name) { return m_members[name]; }
atObject& atObject::operator[](const char *name) { return Get(name); }
atObject& atObject::operator[](const atString &name) { return Get(name); }
const atObject& atObject::Get(const atString &name) const { return m_members[name]; }
const atObject& atObject::operator[](const char *name) const { return Get(name); }
const atObject& atObject::operator[](const atString &name) const { return Get(name); }
bool atObject::operator!=(const atObject &value) const { return !(value == *this); }
