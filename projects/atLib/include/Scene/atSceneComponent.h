#ifndef atSceneComponent_h__
#define atSceneComponent_h__

#include "atString.h"
#include "atHashMap.h"
#include "atThreading.h"
#include "atTransformable.h"

class atScene;
class atSceneNode;

class atSceneComponent
{
  friend atSceneNode;

protected:
  // Only classes derived from this should be instantiated
  atSceneComponent(const int64_t &typeID, const atString &typeString);

public:
  virtual bool OnInit();
  virtual bool OnUpdate();
  virtual bool OnRender();
  virtual bool OnDestroy();

  // Get the parent node
  atSceneNode* GetNode() const;
  atScene* GetScene() const;

  // Get the global transform of the parent node
  atTransformable<double> GlobalTransform(const bool &reEvaluate = false);

  // Get the local transform of the parent node
  atTransformable<double>& Transform() const;

  // Get this component as the specified type.
  // Returns nullptr if the provided type does typeID.
  template<typename T> T* As(const bool &allowDerived = true);
  template<typename T> const T* As(const bool &allowDerived = true) const;

  // Check if the component pointer is of a derived type T
  template<typename T> bool Is() const;

  // Check if the component is derived from the type T
  template<typename T> bool IsDerived() const;

  virtual bool IsDerived(const int64_t &typeID) const = 0;

  static int64_t TypeID();

  const int64_t  typeID;
  const atString typeString;
  
protected:
  static bool _IsDerived(const int64_t &typeID);

private:
  atSceneNode* m_pNode;
};

#include "atSceneComponent.inl"

// When creating a new component type, use this to implement the required functions
#define atImplementSceneComponent(type, base, stringID)                                                    \
  friend class atSceneComponentFactory;                                                                    \
  friend class atSceneNode;                                                                                \
protected:                                                                                                 \
  static bool _IsDerived(const int64_t &typeID) { return BaseID() == typeID || base::_IsDerived(typeID); } \
  type(const int64_t &instanceTypeID = TypeID(), const atString &instanceTypeName = TypeString())          \
    : base(instanceTypeID, instanceTypeName)                                                               \
  {}                                                                                                       \
public:                                                                                                    \
  static int64_t Register() { return atSceneComponentFactory::RegisterComponent<type>(stringID); }         \
  static int64_t TypeID()   { return atSceneComponentFactory::GetComponentID<type>(); }                    \
  static int64_t BaseID()   { return atSceneComponentFactory::GetComponentID<base>(); }                    \
  static const atString& TypeString() { static const atString &typeString = stringID; return typeString; } \
  bool IsDerived(const int64_t &typeID) const override { return _IsDerived(typeID); }                      \
private:                                                                                                   \


#endif // atSceneComponent_h__
