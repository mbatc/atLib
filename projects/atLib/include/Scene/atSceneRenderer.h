#ifndef atSceneRenderer_h__
#define atSceneRenderer_h__

#include "atScene.h"

class atSceneRenderer
{
public:
  atSceneRenderer() = delete;

  static bool Render(atScene *pScene);
  static bool Render(atSceneNode *pNode, const atMat4D &vp);
};

#endif // atSceneRenderer_h__
