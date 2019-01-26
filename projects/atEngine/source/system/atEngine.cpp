#include "atEngine.h"
#include "atInput.h"
#include "atSceneRenderer.h"

atEngine::atEngine(int argc, char **argv)
  : m_result(-1)
  , m_pWindow(nullptr)
  , m_pCamera(nullptr)
{
  if(Initialise(argc, argv))
    m_result = Run();
}

atEngine::~atEngine() { Shutdown(); }

bool atEngine::Initialise(int argc, char **argv)
{
  atUnused(argc, argv);
  m_pWindow = atNew<atWindow>("atEngine");
  m_pCamera = atNew<atSimpleCamera>(*m_pWindow);
  return true;
}

void atEngine::Shutdown()
{
  if(m_pWindow) atDelete(m_pWindow);
  m_pWindow = nullptr;
}

int64_t atEngine::Run()
{
  int64_t res = 0;
  while (atInput::Update() && res == 0)
  {
    m_pWindow->Clear(0xFF333333);

    if (!Update() || !Render())
      res = 1;
    m_pWindow->Swap();
  }
  return 0;
}

int64_t atEngine::Result() const { return m_result; }

bool atEngine::Update()
{
  bool result = m_scene.Update();
  return result;
}

bool atEngine::Render()
{
  bool result = atSceneRenderer::Render(m_scene.GetRoot(), m_pCamera->ProjectionMat() * m_pCamera->ViewMat());
  return result;
}
