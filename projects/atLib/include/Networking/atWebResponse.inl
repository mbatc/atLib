template<typename T>
inline atWebResponseInterface<T>::atWebResponseInterface()
  : m_typeID(typeid(T))
{}

template<typename T>
inline std::type_index atWebResponseInterface<T>::TypeID() const
{
  return m_typeID;
}

template<typename T>
inline bool atWebResponse::Is() const
{
  return m_typeID == typeid(T);
}

template<typename T>
inline const T& atWebResponse::As() const
{
  return *(T*)this;
}

template<typename T>
inline T& atWebResponse::As()
{
  return *(T*)this;
}
