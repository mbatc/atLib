#ifndef atEEditor_h__
#define atEEditor_h__

#include "atEngineUI.h"
#include "atWindow.h"
#include "atGraphics.h"
#include "atCamera.h"

class atEEditor
{
  atEEditor();

public:
  static atEEditor* Instance();
  static void Destroy();

  void Update();
  void Render();

protected:
  atEngineUI m_ui;
};

#endif // atEEditor_h__
