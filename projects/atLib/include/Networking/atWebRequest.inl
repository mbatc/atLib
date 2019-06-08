template<typename T>
inline atWebRequestInterface<T>::atWebRequestInterface()
  : m_typeID(typeid(T))
{}

template<typename T>
inline std::type_index atWebRequestInterface<T>::TypeID() const
{
  return m_typeID;
}

template<typename T>
inline bool atWebRequest::Is() const
{
  return m_typeID == typeid(T);
}

template<typename T>
inline const T& atWebRequest::As() const
{
  return *(T*)this;
}

template<typename T>
inline T& atWebRequest::As()
{
  return *(T*)this;
}