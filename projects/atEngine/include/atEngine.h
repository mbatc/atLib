#ifndef atEngine_h__
#define atEngine_h__

#include "atTypes.h"
#include "atScene.h"
#include "atWindow.h"
#include "atCamera.h"

class atEngine
{
public:
  atEngine(int argc = 0, char **argv = nullptr);
  ~atEngine();

  int64_t Result() const;

protected:
  bool Update();
  bool Render();

  bool Initialise(int argc, char **argv);
  void Shutdown();
  int64_t Run();

  atSimpleCamera *m_pCamera;
  atWindow *m_pWindow;
  int64_t m_result;

  atScene m_scene;
};

#endif // atEngine_h__
