#include "atEngine.h"
#include "atInput.h"
#include "atImGui.h"
#include "atSceneRenderer.h"
#include "atRenderState.h"

atEngine::atEngine(int argc, char **argv)
  : m_result(-1)
  , m_pWindow(nullptr)
  , m_pCamera(nullptr)
  , m_script("packages")
{
  if(Initialise(argc, argv))
    m_result = Run();
}

bool atEngine::Initialise(int argc, char **argv)
{
  atUnused(argc, argv);
  m_pWindow = atNew<atWindow>("atEngine");
  m_pCamera = atNew<atSimpleCamera>(m_pWindow);
  m_script.DoStartupEvent();
  return true;
}

void atEngine::Shutdown()
{
  m_script.DoCleanupEvent();
  if (m_pWindow) atDelete(m_pWindow);
  if (m_pCamera) atDelete(m_pCamera);
  m_pWindow = nullptr;
  m_pCamera = nullptr;
}

int64_t atEngine::Run()
{
  int64_t res = 0;
  atRenderState rs;
  rs.SetRenderTarget(m_pWindow);
  while (atInput::Update() && res == 0)
  {
    m_pWindow->Clear(0xFF333333);

    BeginGUI();
    if (!Update() || !Render())
      res = 1;
    EndGUI();
    m_pWindow->Swap();
  }
  return 0;
}

bool atEngine::Update()
{
  bool result = m_script.DoUpdateEvent(0.0016);
  result &= m_scene.Update();
  return true;
}

bool atEngine::Render()
{
  atMat4D vp = m_pCamera->ProjectionMat() * m_pCamera->ViewMat();
  bool result = m_script.DoDrawEvent(vp);
  result &= atSceneRenderer::Render(m_scene.GetRoot(), vp);
  return true;
}

bool atEngine::BeginGUI()
{
  atImGui::BeginFrame(m_pWindow);
  return m_script.DoGUIEvent();
}

bool atEngine::EndGUI() { return atImGui::EndFrame() && atImGui::Render(); }
int64_t atEngine::Result() const { return m_result; }
atEngine::~atEngine() { Shutdown(); }
