#include "atEScripting.h"
#include "atFilename.h"
#include "atInput.h"

void _ExposeScriptCommands(atEScripting *pLua);

atEScripting::atEScripting(const atString &packageDir)
  : m_running(true)
  , m_pLua(nullptr)
{
  Initialise(packageDir);
}

atEScripting::~atEScripting() { Destroy(); }

bool atEScripting::DoFunction(const atString &name, const atVector<atString> &args, const bool overrideRunning)
{
  if (!m_running && !overrideRunning)
    return false;
  atString argText = args.size() == 0 ? "" : ", ";
  for (const atString &a : args)
    argText += (&a - args.begin()) == args.size() - 1 ? a : a + ", ";
  return RunText("atCore.Call([[" + name + "]]" + argText + ")");
}

bool atEScripting::RunText(const atString &text, const bool overrideRunning)
{
  if (!m_running && !overrideRunning || !m_pLua)
    return false;
  m_running &= m_pLua->RunText(text);
  return m_running;
}

bool atEScripting::Initialise(const atString &packageDir)
{
  Destroy();
  m_running = true;
  m_packageDir = packageDir;
  m_pLua = atNew<atLua>();
  RunText("require([[" + m_packageDir + "/core]])");
  RunText("atCore.Initialise([[" + m_packageDir + "]], [[" + atFilename(packageDir).Name() +"]])");
  
  if (m_running)
  {
    // Expose required atLib functions
    m_pLua->ExposeMathTypes();
    m_pLua->ExposeMathFunctions();
    m_pLua->ExposeScene();
    m_pLua->ExposeImGui();

    // Expose atEngine script commands
    _ExposeScriptCommands(this);
  }

  return DoStartupEvent();
}

void atEScripting::Destroy()
{
  DoCleanupEvent();
  if (m_pLua)
    atDelete(m_pLua);
  m_packageDir = "";
  m_pLua = nullptr;
}

bool atEScripting::Update()
{
  if (atInput::KeyPressed(atKC_Apostraphe))
    Reload();
  return true;
}

bool atEScripting::DoUpdateEvent(const double dt) { return DoFunction("OnUpdate"); }
bool atEScripting::DoDrawEvent(const atMat4D &mvp) { return DoFunction("OnDraw"); }
bool atEScripting::DoStartupEvent() { return DoFunction("OnStartup"); }
bool atEScripting::DoCleanupEvent() { return DoFunction("OnCleanup"); }
bool atEScripting::DoGUIEvent() { return DoFunction("OnGui"); }
bool atEScripting::IsRunning() { return m_running; }
bool atEScripting::Reload() { return Initialise(atString(m_packageDir)); }

// Global Scripting Engine Commands

#include "atEngine.h"

atLuaScene atEScripting::Commands::GetScene()
{
  return atEngine::Commands::Scene();
}

atVec2D atEScripting::Commands::GetWindowSize()
{
  return atEngine::Commands::WindowSize();
}

double atEScripting::Commands::GetWindowWidth()
{
  return (double)atEngine::Commands::WindowWidth();
}

double atEScripting::Commands::GetWindowHeight()
{
  return (double)atEngine::Commands::WindowHeight();
}

void _ExposeScriptCommands(atEScripting *pScripting)
{
  pScripting->ExposeFunction("GetScene", atEScripting::Commands::GetScene);
  pScripting->ExposeFunction("GetWindowWidth", atEScripting::Commands::GetWindowWidth);
  pScripting->ExposeFunction("GetWindowHeight", atEScripting::Commands::GetWindowHeight);
  pScripting->ExposeFunction("GetWindowSize", atEScripting::Commands::GetWindowSize);
}