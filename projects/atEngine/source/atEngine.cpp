#include "atEngine.h"
#include "atInput.h"
#include "atImGui.h"
#include "atRenderState.h"

atEngine *atEngine::m_pInstance = nullptr;

atEngine::atEngine()
  : m_result(-1)
{
  Initialise();
}

atEngine *atEngine::Instance()
{
  if (!m_pInstance)
    m_pInstance = new atEngine();
  return m_pInstance;
}

void atEngine::Destroy()
{
  if (m_pInstance)
    delete m_pInstance;
  m_pInstance = nullptr;
}

bool atEngine::Initialise() { return true; }

void atEngine::Shutdown() {}

bool atEngine::Update()
{
  return m_scene.Update();
}

bool atEngine::Render()
{
  // return m_scene.Render();
  return true;
}

int64_t atEngine::Result() const { return m_result; }
atEngine::~atEngine() { Shutdown(); }
