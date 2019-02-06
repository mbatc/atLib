#include "atEngine.h"
#include "atInput.h"
#include "atImGui.h"
#include "atSceneRenderer.h"
#include "atRenderState.h"

atEngine *atEngine::m_pInstance = nullptr;

atEngine::atEngine(int argc, char **argv)
  : m_result(-1)
  , m_pWindow(nullptr)
  , m_pCamera(nullptr)
  , m_script("packages")
{
  m_pInstance = this;
  if(Initialise(argc, argv))
    m_result = Run();
}

atEngine *atEngine::Instance(int argc, char **argv)
{
  if (!m_pInstance)
    m_pInstance = new atEngine(argc, argv);
  return m_pInstance;
}

void atEngine::Destroy()
{
  if (m_pInstance)
    delete m_pInstance;
  m_pInstance = nullptr;
}

bool atEngine::Initialise(int argc, char **argv)
{
  atUnused(argc, argv);
  m_pWindow = atNew<atWindow>("atEngine");
  m_pCamera = atNew<atSimpleCamera>(m_pWindow);
  return true;
}

void atEngine::Shutdown()
{
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
  m_script.Update();
  bool result = m_script.DoUpdateEvent(0.0016);
  result &= m_scene.Update();
  return true;
}

bool atEngine::Render()
{
  atMat4D vp = m_pCamera->ProjectionMat() * m_pCamera->ViewMat();
  bool result = m_script.DoDrawEvent(vp);
  result &= m_scene.Draw();
  // result &= atSceneRenderer::Render(m_scene.GetRoot(), vp);
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

// Global Engine Commands

atScene* atEngine::Commands::Scene() 
{
  return &Instance()->m_scene; 
}

const atVec2I& atEngine::Commands::WindowSize() 
{ 
  return Instance()->m_pWindow->Size(); 
}

const int64_t atEngine::Commands::WindowWidth()
{
  return (int64_t)Instance()->m_pWindow->Width();
}

const int64_t atEngine::Commands::WindowHeight()
{
  return (int64_t)Instance()->m_pWindow->Height();
}
