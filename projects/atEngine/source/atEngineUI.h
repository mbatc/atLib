#ifndef atEngineUI_h__
#define atEngineUI_h__

#include "atEUIComponent.h"

class atEngineUI
{
public:
  atEngineUI();

  void Update();
  bool Draw();

  atCanvas2D& UICanvas();
  atCanvas2D& Foreground();
  atCanvas2D& Background();

protected:
  atEUIComponent *m_pRoot = nullptr;

  atCanvas2D m_canvas;
  atCanvas2D m_foreground;
  atCanvas2D m_background;
};

#endif // atEUI_h__
