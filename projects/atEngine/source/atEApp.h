#ifndef atEApp_h__
#define atEApp_h__

#include "atWindow.h"
#include "atFilename.h"

class atEApp
{
  atEApp();
public:
  static atEApp* Instance();
  static void Destroy();

  bool Initialise(int argc, char **argv);
  int Run();

  atFilename GetExeDir() const;
  atFilename GetExePath() const;
  atFilename GetAppDataPath() const;

  const atVector<atString>& GetCmdArgs() const;

  atWindow* Window() const;
  atGraphics* Graphics() const;

protected:
  atVector<atString> m_args;

  atFilename m_appData;
  atFilename m_exePath;

  atWindow *m_pWindow;
  atGraphics *m_pGraphics;
};

#endif // atEApp_h__
