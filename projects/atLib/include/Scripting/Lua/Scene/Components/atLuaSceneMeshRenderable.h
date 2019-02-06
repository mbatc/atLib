#ifndef atLuaSceneMeshRenderable_h__
#define atLuaSceneMeshRenderable_h__

#include "atLuaSceneComponent.h"

class atSceneMeshRenderable;

class atLuaSceneMeshRenderable : public atLuaSceneComponent
{
public:
  atLuaSceneMeshRenderable(const atLuaSceneComponent &comp);
  atLuaSceneMeshRenderable(atSceneComponent *pComp);
  atLuaSceneMeshRenderable(const atLuaSceneMeshRenderable &copy);
  atLuaSceneMeshRenderable(atLuaSceneMeshRenderable &&move);

  atLuaSceneNode Node();
  bool SetModelPath(const char *file);
  const char* GetModelPath();

  atSceneMeshRenderable* Mesh();
  bool IsValid();
};

#endif // atLuaSceneMeshRenderable_h__
