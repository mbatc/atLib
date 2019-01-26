#ifndef atEScripting_h__
#define atEScripting_h__

#include "atLua.h"

class atEScripting
{
public:
  atEScripting(const atString &packageDir);
  ~atEScripting();

  bool Reload();
  bool IsRunning();

  bool DoStartupEvent();
  bool DoCleanupEvent();
  bool DoUpdateEvent(const double dt);
  bool DoDrawEvent(const atMat4D &mvp);
  bool DoGUIEvent();
  bool DoFunction(const atString name, const atVector<atString> &args = {}, const bool overrideRunning = false);

protected:
  bool RunText(const atString &text, const bool overrideRunning = false);
  bool Initialise(const atString &packageDir);
  void Destroy();

  atString m_packageDir;
  bool m_running;
  atLua *m_pLua;
};

#endif // atEScripting_h__
