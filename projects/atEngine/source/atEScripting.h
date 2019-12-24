#ifndef atEScripting_h__
#define atEScripting_h__

#include "atPython.h"

class atScriptComponent;

class atEScripting
{
protected:
  atEScripting();
  ~atEScripting();

  static atEScripting *m_pInstance;

public:
  static atEScripting* Instance();
  static void Destroy();

  bool EngineOnUpdate();
  bool EngineOnDraw();

protected:
  void SetPyInterpreter(PyThreadState *pState);

  PyThreadState *m_pCurrent = nullptr;
  PyThreadState *m_pEngineState = nullptr;
  PyThreadState *m_pSceneState = nullptr;

  static bool InitCallback(atScriptComponent *pComponent);
  static bool UpdateCallback(atScriptComponent *pComponent, const double &deltaTime);
  static bool RenderCallback(atScriptComponent *pComponent, const atMat4D &vp);
  static bool DestroyCallback(atScriptComponent *pComponent);
};

#endif // atEScripting_h__
