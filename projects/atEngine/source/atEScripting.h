#ifndef atEScripting_h__
#define atEScripting_h__

#include "sol.hpp"

#include "atLua.h"
#include "atLuaScene.h"

class atEScripting
{
public:
  atEScripting(const atString &packageDir);
  ~atEScripting();

  class Commands
  {
  public:
    static atLuaScene GetScene();

    static atVec2D GetWindowSize();
    static double GetWindowWidth();
    static double GetWindowHeight();

    static const char* DialogOpen(const char *initialDir, const char* fileFilter, int64_t initialFilter);
    static const char* DialogSaveAs(const char *initialDir, const char* fileFilter, int64_t initialFilter);
  };

  bool Reload();
  bool Update();
  bool IsRunning();

  bool DoStartupEvent();
  bool DoCleanupEvent();
  bool DoUpdateEvent(const double dt);
  bool DoDrawEvent(const atMat4D &mvp);
  bool DoGUIEvent();
  bool DoFunction(const atString &name, const atVector<atString> &args = {}, const bool overrideRunning = false);

  template <typename R, typename... Args> void ExposeFunction(const atString & name, R(*func)(Args...));

protected:
  bool RunText(const atString &text, const bool overrideRunning = false);
  bool Initialise(const atString &packageDir);
  void Destroy();

  atString m_packageDir;
  bool m_running;
  atLua *m_pLua;
};

template <typename R, typename... Args> inline void atEScripting::ExposeFunction(const atString & name, R(*func)(Args...)) { (*m_pLua->GetLua())["atCore"][name.c_str()] = func; }

#endif // atEScripting_h__
