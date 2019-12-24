#ifndef atEngine_h__
#define atEngine_h__

#include "atEGui.h"
#include "atWindow.h"
#include "atGraphics.h"

class atEngine
{
public:
  atEngine();
  ~atEngine();

  int Run();

protected:
  atWindow m_mainWnd;
  atGraphics m_gfx;
  atEGui m_gui;
};

#endif // atEngine_h__
