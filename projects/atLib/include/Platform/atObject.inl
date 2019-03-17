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
inline void atObjectDestructFunc(void * pData)
{
  T* pObj = (T*)pData;
  pObj->~T();
}