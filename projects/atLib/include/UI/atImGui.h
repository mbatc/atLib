#ifndef atImGui_h__
#define atImGui_h__

#include "atWindow.h"
#include "../../3rdParty/imgui/imgui.h"

class atImGui
{
public:
  static bool BeginFrame(atWindow &wnd);
  static bool EndFrame();
  static bool Render();
  static bool ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif // atImGui_h__
