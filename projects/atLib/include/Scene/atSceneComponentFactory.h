#ifndef atSceneComponentFactory_h__
#define atSceneComponentFactory_h__

#include "atSceneComponent.h"

class atSceneComponentFactory
{
  friend atSceneComponent;
public:
  typedef std::function<atSceneComponent*()> AllocatorT;

  atSceneComponentFactory() = delete;

  // Create a component using a string ID
  // The component must be registered
  static atSceneComponent* Create(const atString &stringID);

  // Get the component ID for the specified type
  template<typename T> static int64_t GetComponentID();

  // Register a component derived from atSceneComponent with the scene factory
  template<typename T> static bool RegisterComponent(const atString &name);

  // static int64_t GetComponentID(const atString &name);
  // template<typename T> static atString GetComponentName();
  // 
  // static atVector<int64_t> ComponentIDs();
  // static atVector<atString> ComponentNames();

private:
  // Get the next available componentID
  // This ID will be unique
  static int64_t NextComponentID();

  // Stores the data associated with a registered component
  struct ComponentInterface
  {
    atString typeID;
    AllocatorT allocator;
  };

  static std::mutex m_lock;

  static atHashMap<atString, int64_t> m_nameToID;
  static atVector<ComponentInterface> m_registry;
};

#include "atSceneComponentFactory.inl"
#endif // atSceneComponentFactory_h__
