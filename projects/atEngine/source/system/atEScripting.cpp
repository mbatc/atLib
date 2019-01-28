#include "atEScripting.h"
#include "atFilename.h"
#include "atInput.h"

atEScripting::atEScripting(const atString &packageDir)
  : m_running(true)
  , m_pLua(nullptr)
{
  Initialise(packageDir);
}

atEScripting::~atEScripting() { Destroy(); }

bool atEScripting::DoFunction(const atString name, const atVector<atString> &args, const bool overrideRunning)
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
  if (!m_running && !overrideRunning)
    return false;
  m_running &= m_pLua->RunText(text);
  return m_running;
}

bool atEScripting::Initialise(const atString &packageDir)
{
  Destroy();
  m_packageDir = packageDir;
  m_pLua = atNew<atLua>();
  RunText("require([[" + m_packageDir + "/core]])");
  RunText("atCore.Initialise([[" + m_packageDir + "]], [[" + atFilename(packageDir).Name() +"]])");
  
  // Expose required atLib functions
  m_pLua->ExposeMathTypes();
  m_pLua->ExposeMathFunctions();
  m_pLua->ExposeScene();
  m_pLua->ExposeImGui();

  m_packageDir = packageDir;
  return m_running;
}

void atEScripting::Destroy()
{
  if (m_pLua)
    atDelete(m_pLua);
  m_packageDir = "";
  m_pLua = nullptr;
}

bool atEScripting::Update()
{
  if (atInput::KeyDown(atKC_Apostraphe))
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
