#ifndef atSceneNode_h__
#define atSceneNode_h__

#include "atVector.h"
#include "atTransformable.h"
#include "atSceneComponent.h"
#include "atTransformComponent.h"

class atScene;

class atSceneNode
{
  friend atScene;

public:
  atSceneNode(const atString &name = "");
  atSceneNode(atSceneNode &&o);
  atSceneNode(const atSceneNode &o);
  ~atSceneNode();

  const atString& GetName() const;
  void SetName(const atString &name);

  atScene* GetScene() const;
  atSceneNode* GetParent() const;
  atTransformComponent* GetTransform() const;

  int64_t ChildCount() const;
  atSceneNode* GetChild(const int64_t &index) const;
  
  bool Init();
  bool Update(const double &deltaTime);
  bool Render(const atMat4D &vp);
  bool Destroy();
 
  int64_t GetID() const;
  int64_t ComponentCount() const;
  int64_t ComponentCount(const atCoreComponentType &type) const;

  atSceneComponent* GetComponent(const int64_t &index) const;
  atSceneComponent* GetComponent(const atCoreComponentType &type, const int64_t &index = 0) const;
  atVector<atSceneComponent*> GetComponents(const atCoreComponentType &type) const;

  template<typename T> T* GetComponent() const;
  template<typename T> atVector<T*> GetComponents() const;

  template<typename T> void AddComponent(T &&component);
  template<typename T> void AddComponent(const T &component);
  template<typename T, typename... Args> void AddComponent(Args&&... component);

  void SetScene(atScene *pScene);

protected:
  void AddComponent_Internal(atSceneComponent *pComponent);
  
  atVector<atSceneComponent*> m_components;
  atScene *m_pScene = nullptr;
  atString m_name;
};

template<typename T>
inline void atSceneNode::AddComponent(const T &component) { AddComponent_Internal(atNew<T>(std::forward<T>(component))); }

template<typename T>
inline T* atSceneNode::GetComponent() const { return (T*)GetComponent(T::TypeID()); }

template<typename T>
inline atVector<T*> atSceneNode::GetComponents() const
{
  atVector<T*> components;
  for (atSceneComponent *pComp : components)
    if (pComp->typeID == T::TypeID())
      components.push_back((T*)pComp);
  return components;
}

template<typename T>
inline void atSceneNode::AddComponent(T &&component) { AddComponent_Internal(atNew<T>(std::forward<T>(component))); }

template<typename T, typename ...Args>
inline void atSceneNode::AddComponent(Args&& ...component) { AddComponent_Internal(atNew<T>(std::forward<Args>(component)...)); }

#endif // atSceneNode_h__
