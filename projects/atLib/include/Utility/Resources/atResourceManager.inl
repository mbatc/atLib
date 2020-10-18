
template<typename T>
inline bool atResourceManager::CanHandle()
{
  for (atResourceHandlerBase *pHandler : m_handlers)
    if (pHandler->Handles<T>())
      return true;
  return false;
}

template<typename T>
inline bool atResourceManager::AddHandler()
{
  if (HasHandler<T>())
    false;
  m_handlers.emplace_back(atNew(T));
  m_handlerType.emplace_back(typeid(T));
  m_handlers.back()->m_pManager = this;
  return true;
}

template<typename T>
inline bool atResourceManager::HasHandler()
{
  for (const std::type_index &typeInfo : m_handlerType)
    if (typeInfo == typeid(T))
      return true;
  return false;
}

template<typename T>
inline bool atResourceManager::RemoveHandle()
{
  for (int64_t i = 0; i < m_handlers.size(); ++i)
    if (m_handlerType[i] == typeid(T))
    {
      atDelete(m_handlers[i]);
      m_handlerType.erase(i);
      m_handlers.erase(i);
      return true;
    }
  return false;
}

template<typename T>
inline atResourceHandleT<T> atResourceManager::Request(const atObjectDescriptor &request)
{
  for (atResourceHandlerBase *pHandler : m_handlers)
    if (pHandler->Handles<T>())
    {
      atResourceHandle resource = pHandler->Request(request);
      if (resource.HasResource())
        return resource;
    }
  return atResourceHandle();
}
