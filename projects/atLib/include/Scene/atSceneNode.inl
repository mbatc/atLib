
template<typename T> inline T* atSceneNode::GetComponent() const { return (T*)GetComponent(FindComponent<T>()); }

template<typename T>
inline int64_t atSceneNode::FindComponent() const
{
  for (int64_t i = 0; i < m_components.size(); ++i)
    if (m_components[i]->typeID == T::TypeID())
      return i;
  return -1;
}

template<typename T> inline T* atSceneNode::AddComponent() { return HasComponent<T>() ? nullptr : (T*)AddComponent(new T()); }

template<typename T> inline bool atSceneNode::HasComponent() const { return GetComponent<T>() != nullptr; }

template<typename T> inline bool atSceneNode::RemoveComponent() { return RemoveComponent(FindComponent<T>()); }
