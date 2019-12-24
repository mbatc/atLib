#include "atEScripting.h"
#include "atScenePythonAPI.h"
#include "atScriptComponent.h"

atEScripting *atEScripting::m_pInstance = nullptr;

atEScripting::atEScripting()
{
  atPython::AddModule("atScene", atScenePythonAPI_Init);
  atPython::Init();

  PyGILState_STATE lock = PyGILState_Ensure();
  m_pEngineState = PyThreadState_Get();
  PyGILState_Release(lock);

  m_pSceneState = Py_NewInterpreter();
  atPython::Run("import atEngine");

  // Setup scene scripting callbacks
  atScriptComponent::InitCallback = &atEScripting::InitCallback;
  atScriptComponent::UpdateCallback = &atEScripting::UpdateCallback;
  atScriptComponent::RenderCallback = &atEScripting::RenderCallback;
  atScriptComponent::DestroyCallback = &atEScripting::DestroyCallback;
}

atEScripting::~atEScripting() { atPython::Shutdown(); }

atEScripting* atEScripting::Instance()
{
  if (!m_pInstance)
    m_pInstance = new atEScripting();
  return m_pInstance;
}

void atEScripting::Destroy()
{
  if (m_pInstance)
    delete m_pInstance;
  m_pInstance = nullptr;
}

bool atEScripting::EngineOnUpdate()
{
  SetPyInterpreter(m_pEngineState);
  return atPython::Run("atEngine.Core.Update()") == atPyR_Success;
}

bool atEScripting::EngineOnDraw()
{
  SetPyInterpreter(m_pEngineState);
  return atPython::Run("atEngine.Core.Draw()") == atPyR_Success;
}

void atEScripting::SetPyInterpreter(PyThreadState *pState)
{
  if (pState == m_pCurrent)
    return;

  PyThreadState_Swap(pState);
  m_pCurrent = pState;
}

bool atEScripting::InitCallback(atScriptComponent *pComponent)
{
  atEScripting *pInstance = Instance();
  pInstance->SetPyInterpreter(pInstance->m_pSceneState);

  // Instantiate an instance of pComponents script
  atPython::Run(pComponent->m_script);
  return true;
}

bool atEScripting::UpdateCallback(atScriptComponent *pComponent, const double &deltaTime)
{
  atEScripting *pInstance = Instance();
  pInstance->SetPyInterpreter(pInstance->m_pSceneState);
  atPython::Run("");
  return true;
}

bool atEScripting::RenderCallback(atScriptComponent *pComponent, const atMat4D &vp)
{
  atEScripting *pInstance = Instance();
  pInstance->SetPyInterpreter(pInstance->m_pSceneState);
  atPython::Run("");
  return true;
}

bool atEScripting::DestroyCallback(atScriptComponent *pComponent)
{
  atEScripting *pInstance = Instance();
  pInstance->SetPyInterpreter(pInstance->m_pSceneState);
  atPython::Run("");
  return true;
}
