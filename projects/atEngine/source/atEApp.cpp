#include "atEApp.h"
#include "atFileSystem.h"
#include "atRenderState.h"
#include "atInput.h"
#include "atEEditor.h"
#include "atEngine.h"

static atEApp *_pInstance = nullptr;

atEApp::atEApp() {}

atEApp* atEApp::Instance()
{
  if (!_pInstance)
    _pInstance = new atEApp;
  return _pInstance;
}

void atEApp::Destroy()
{
  if (_pInstance)
    delete _pInstance;
  _pInstance = nullptr;
}

bool atEApp::Initialise(int argc, char ** argv)
{
  // Get cmd arguments
  for (int i = 0; i < argc; ++i)
    m_args.push_back(argv[i]);

  // Setup directories
  m_appData = atFileSystem::GetDirectory_AppData().Path() + "/atEngine/";
  char path[MAX_PATH + 1] = { 0 };
  GetModuleFileName(NULL, path, MAX_PATH);
  m_exePath = path;

  // Setup window
  atWindowCreateInfo wci;
  wci.title = "atEngine";
  m_pWindow = atNew<atWindow>(wci);
  m_pGraphics = atGraphics::Create(atGfxApi_OpenGL, m_pWindow);

  return true;
}

int atEApp::Run()
{
  int64_t res = 0;

  atRenderState rs;
  // rs.SetRenderTarget(m_pWindow);
  while (atInput::Update() && res == 0)
  {
    rs.SetViewport(atVec4I(0, 0, m_pWindow->Size()));
    m_pWindow->Clear(0xFF333333);

    // Update and render the UI
    atEEditor::Instance()->Update();
    atEEditor::Instance()->Render();

    // Update and render the engine
    atEngine::Instance()->Update();
    atEngine::Instance()->Render();

    m_pWindow->Swap();
  }
  return 0;
}

atFilename atEApp::GetExeDir() const { return m_exePath.Directory(); }
atFilename atEApp::GetExePath() const { return m_exePath; }
atFilename atEApp::GetAppDataPath() const { return m_appData; }
const atVector<atString>& atEApp::GetCmdArgs() const { return m_args; }

atWindow* atEApp::Window() const { return m_pWindow; }
atGraphics* atEApp::Graphics() const { return m_pGraphics; }
