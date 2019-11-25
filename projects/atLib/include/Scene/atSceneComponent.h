#ifndef atSceneComponent_h__
#define atSceneComponent_h__

#include "atCoreComponents.h"
#include "atString.h"
#include <typeindex>

class atScene;
class atSceneNode;

class atSceneComponent
{
  friend atSceneNode;

public:
  atSceneComponent(const atString &typeName, const atString &name = "");
  atSceneComponent(const atCoreComponentType &type, const atString &name = "");

  virtual bool Init();
  virtual bool Update(const double &deltaTime);
  virtual bool Render(const atMat4D &vp);
  virtual bool Destroy();

  virtual atSceneComponent* Copy() const = 0;

  const atCoreComponentType typeID;

  static atCoreComponentType TypeID();

  void SetName(const atString &name);
  const atString& GetName() const;

  atScene* GetScene() const;
  atSceneNode* GetNode() const;

protected:
  void SetNode(atScene *pScene, atSceneNode *pNode);

  atString m_name;
  atScene *m_pScene = nullptr;
  atSceneNode *m_pNode = nullptr;
};

#endif // atSceneComponent_h__
