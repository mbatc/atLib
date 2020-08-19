
template<typename T>
int64_t atSceneComponentFactory::GetComponentID()
{
  static_assert(std::is_same<atSceneComponent, T>::value || std::is_base_of<atSceneComponent, T>::value, "T must be derived from atSceneComponent");
  static const int64_t id = NextComponentID(); // Retrieve an ID once per type this function is instantiated for
  return id;
}

template<typename T>
bool atSceneComponentFactory::RegisterComponent(const atString &name)
{
  static_assert(std::is_base_of<atSceneComponent, T>::value, "T must be derived from atSceneComponent");

  int64_t id = GetComponentID<T>();

  atScopeLock lock(m_lock);
  if (!m_nameToID.TryAdd(name, id))
    return false; // A component with that name has already been registered

  ComponentInterface intr;
  intr.typeID = name;
  intr.allocator = []() { return new T(); };

  m_registry.resize(atMax(id + 1, m_registry.size()));
  m_registry[id] = intr;
  return true;
}
