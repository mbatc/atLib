#ifndef atObject_h__
#define atObject_h__

#include "atString.h"
#include "atHashMap.h"

class atObject
{
public:
  atObject();
  atObject(atObject &&move);
  atObject(const atObject &copy);
  ~atObject();

  void Assign(const atObject &value);
  bool AddMember(const atString &name, const atObject &value);
  template <typename T> void Assign(const T &value);
  template <typename T> bool AddMember(const atString &name, const T &value);

  const atObject& operator=(const atObject &copy);
  const atObject& operator=(atObject &&move);

  bool operator==(const atObject &value) const;
  bool operator!=(const atObject &value) const;
  template <typename T> const atObject& operator=(const T &value);
  template <typename T> bool operator==(const T &value) const;
  template <typename T> bool operator!=(const T &value) const;

  atObject& Get(const atString &name);
  atObject& operator [](const char *name);
  atObject& operator [](const atString &name);
  const atObject& Get(const atString &name) const;
  const atObject& operator [](const char *name) const;
  const atObject& operator [](const atString &name) const;

  template <typename T> operator T();

protected:
  atTypeDesc m_desc;
  atVector<uint8_t> m_data;
  atHashMap<atString, atObject> m_members;
};

template<typename T> void atObject::Assign(const T &value)
{
  atAssert(m_data.size() == 0 || (atGetType<T>() == m_desc.type && sizeof(T) == m_data.size()), "Assigning an atObject of different type or size");
  m_desc = atGetTypeDesc(value);
  m_data.resize(sizeof(T));
  memcpy(m_data.data(), &value, (size_t)m_data.size());
}

template<typename T> bool atObject::AddMember(const atString &name, const T &value)
{
  if (m_members.Contains(name))
    return false;
  m_members.Add(name, atObject());
  m_members[name].Assign(value);
  return true;
}

template<typename T> bool atObject::operator==(const T &value) const
{
  atObject val;
  val.Assign(value);
  return val == *this;
}

template<typename T> atObject::operator T()
{
  atAssert(atGetTypeDesc<T>() == m_desc, "Cannot cast atObject to type T");
  return *(T*)m_data.data();
}

template<typename T> const atObject& atObject::operator=(const T &value) { Assign(value); return *this; }
template<typename T> bool atObject::operator!=(const T &value) const { return !(value == *this); }

#endif // atObject_h__
