#include "atPython.h"
#include "atEngine.h"
#include "atInput.h"
#include "atScenePythonAPI.h"

atEngine::atEngine()
  : m_mainWnd("atEngine")
  , m_gfx(&m_mainWnd)
{
  atPython::AddModule("atScene", atScenePythonAPI_Init);
  atPython::Init();
  PyThreadState *pPyState = Py_NewInterpreter();
  PyThreadState_Swap(pPyState);
}

atEngine::~atEngine()
{
  atPython::Shutdown();
}

int atEngine::Run()
{
  while (atInput::Update())
  {
    m_gui.Update(&m_mainWnd);

    m_mainWnd.Clear(0xFF333333);
    m_gui.Draw();
    m_mainWnd.Swap();
  }

  return 0;
}