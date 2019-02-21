#ifndef atEngine_h__
#define atEngine_h__

#include "atTypes.h"
#include "atScene.h"
#include "atWindow.h"
#include "atSceneCamera.h"
#include "atEScripting.h"

class atEngine
{
  static atEngine *m_pInstance;
  atEngine(int argc = 0, char **argv = nullptr);
  ~atEngine();
public:
  class Commands
  {
  public:
    static atScene* Scene();
    static const atVec2I& WindowSize();
    static const int64_t WindowWidth();
    static const int64_t WindowHeight();
  };

  // argc and argv are only used if the engine is begin created
  static atEngine* Instance(int argc = 0, char **argv = nullptr);
  static void Destroy();

  int64_t Result() const;

protected:
  bool Update();
  bool Render();
  bool BeginGUI();
  bool EndGUI();

  bool Initialise(int argc, char **argv);
  void Shutdown();
  int64_t Run();

  atSimpleCamera *m_pCamera;
  atWindow *m_pWindow;
  int64_t m_result;

  atEScripting m_script;
  atScene m_scene;
};

#endif // atEngine_h__
