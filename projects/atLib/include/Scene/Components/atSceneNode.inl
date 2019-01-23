template <typename T> inline atVector<T*> atSceneNode::Components()
{
  static_assert(std::is_base_of<atSceneComponent, T>::value, "Invalid Component Type: T must be derived from atSceneComponent");
  atVector<T*> ret;
  for (atSceneComponent *pComp : m_components)
    if (T::typeID == pComp->TypeID())
      ret.push_back((T*)pComp);
  return ret;
}

template <typename T> inline T* atSceneNode::AddComponent(T *pComponent)
{
  static_assert(std::is_base_of<atSceneComponent, T>::value, "Invalid Component Type: T must be derived from atSceneComponent");
  pComponent->m_pNode = this;
  m_components.push_back(pComponent);
  return pComponent;
}

template <typename T> inline T* atSceneNode::AddComponent() { return AddComponent(atNew<T>()); }
