#ifndef atEngine_h__
#define atEngine_h__

#include "atTypes.h"
#include "atScene.h"
#include "atWindow.h"
#include "atCamera.h"
#include "atEngineUI.h"
#include "atResourceManager.h"

class atEngine
{
  static atEngine *m_pInstance;

  atEngine();
  ~atEngine();
public:
  // argc and argv are only used if the engine is begin created
  static atEngine* Instance();
  static void Destroy();

  int64_t Result() const;

  bool Update();
  bool Render();

protected:
  bool Initialise();
  void Shutdown();

  int64_t m_result;
  atScene m_scene;
  atResourceManager m_resources;
};

#endif // atEngine_h__
