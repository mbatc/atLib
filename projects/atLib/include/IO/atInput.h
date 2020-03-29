
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#ifndef _atInput_h__
#define _atInput_h__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <atPlatform.h>
#include "atButtonState.h"
#include "atString.h"

enum atKeyCode
{
  atKC_0 = '0',
  atKC_1 = '1',
  atKC_2 = '2',
  atKC_3 = '3',
  atKC_4 = '4',
  atKC_5 = '5',
  atKC_6 = '6',
  atKC_7 = '7',
  atKC_8 = '8',
  atKC_9 = '9',
  atKC_A = 'A',
  atKC_B = 'B',
  atKC_C = 'C',
  atKC_D = 'D',
  atKC_E = 'E',
  atKC_F = 'F',
  atKC_G = 'G',
  atKC_H = 'H',
  atKC_I = 'I',
  atKC_J = 'J',
  atKC_K = 'K',
  atKC_L = 'L',
  atKC_M = 'M',
  atKC_N = 'N',
  atKC_O = 'O',
  atKC_P = 'P',
  atKC_Q = 'Q',
  atKC_R = 'R',
  atKC_S = 'S',
  atKC_T = 'T',
  atKC_U = 'U',
  atKC_V = 'V',
  atKC_W = 'W',
  atKC_X = 'X',
  atKC_Y = 'Y',
  atKC_Z = 'Z',
  atKC_Equals = '=',
  atKC_Apostraphe = VK_OEM_7,
  atKC_OpenSqrBracket = '[',
  atKC_CloseSqrBracket = ']',
  atKC_Period = VK_DECIMAL,
  atKC_Comma = ',',
  atKC_ForwardSlash = '\\',
  atKC_BackSlash = '/',
  atKC_Tab = VK_TAB,
  atKC_CapsLock = VK_CAPITAL,
  atKC_Shift = VK_SHIFT,
  atKC_LShift = VK_LSHIFT,
  atKC_RShift = VK_RSHIFT,
  atKC_Control = VK_CONTROL,
  atKC_LControl = VK_LCONTROL,
  atKC_RControl = VK_RCONTROL,
  atKC_Return = VK_RETURN,
  atKC_Backspace = VK_BACK,
  atKC_LAlt = VK_LMENU,
  atKC_RAlt = VK_RMENU,
  atKC_Space = VK_SPACE,
  atKC_Delete = VK_DELETE,
  atKC_Home = VK_HOME,
  atKC_Insert = VK_INSERT,
  atKC_PageDown = VK_NEXT,
  atKC_PageUp = VK_PRIOR,
  atKC_End = VK_END,
  atKC_ScrollLock = VK_SCROLL,
  atKC_PauseBreak = VK_PAUSE,
  atKC_PrintScreen = VK_PRINT,
  atKC_NumLock = VK_NUMLOCK,
  atKC_Left = VK_LEFT,
  atKC_Right = VK_RIGHT,
  atKC_Up = VK_UP,
  atKC_Down = VK_DOWN,
  atKC_Escape = VK_ESCAPE,
  atKC_F1 = VK_F1,
  atKC_F2 = VK_F2,
  atKC_F3 = VK_F3,
  atKC_F4 = VK_F4,
  atKC_F5 = VK_F5,
  atKC_F6 = VK_F6,
  atKC_F7 = VK_F7,
  atKC_F8 = VK_F8,
  atKC_F9 = VK_F9,
  atKC_F10 = VK_F10,
  atKC_F11 = VK_F11,
  atKC_F12 = VK_F12,
  atKC_Menu = VK_MENU,

  atKC_Count = 256,
  
  atKC_MB_Left,
  atKC_MB_Right,
  atKC_MB_Middle,

  atKC_MB_Count = 3,

  atKC_ImGui_Left = 0,
  atKC_ImGui_Right = 1,
  atKC_ImGui_Middle = 2,
};

class atWindow;

class atInput
{
  friend atWindow;
public:
  static bool Update(const bool escExit = true, atWindow *pWindow = nullptr);
  
  // Update button inputs
  static void OnButtonDown(const int64_t keyCode, const double dt);
  static void OnButtonUp(const int64_t keyCode, const double dt);
  static void OnMouseMove(const atVec2I &pos, const double dt);

  // Get Mouse/Key inputs using an atKeyCode
  static const atButtonState &GetButton(int64_t keyCode);
  static bool ButtonDown(const int64_t key);
  static bool ButtonUp(const int64_t key);
  static bool ButtonPressed(const int64_t key);
  static bool ButtonReleased(const int64_t key);
  static double ButtonDownTime(const int64_t key);
  static double ButtonUpTime(const int64_t key);
  
  // Get mouse inputs
  static double LeftMouseUpTime();
  static double LeftMouseDownTime();
  static bool LeftMouseDown();
  static bool LeftMouseUp();
  static bool LeftMousePressed();
  static bool LeftMouseReleased();

  static double RightMouseUpTime();
  static double RightMouseDownTime();
  static bool RightMouseDown();
  static bool RightMouseUp();
  static bool RightMousePressed();
  static bool RightMouseReleased();

  static double MiddleMouseUpTime();
  static double MiddleMouseDownTime();
  static bool MiddleMouseDown();
  static bool MiddleMouseUp();
  static bool MiddleMousePressed();
  static bool MiddleMouseReleased();

  static void LockMouse(const bool lock, const atVec2I &pos = { 0, 0 });
  static bool IsMouseLocked();

  static atVec2I MouseDelta();
  static atVec2F MouseDirection();
  static void OnMouseWheel(const float scroll);
  static void OnMouseWheelH(const float scroll);
  static const float MouseScrollX();
  static const float MouseScrollY();
  static const atVec2F& MouseScroll();
  static const atVec2F& MousePos();
  static const atVec2F& MouseVelocity();
  static void SetMousePos(const atVec2I& pos, const bool updateLastPos = true);

  static bool MouseMoved();

  static void SetDT(const double dt);

  static atString ToString(const int64_t code);

  static atVector<atString> DroppedFiles();

protected:
  static void RegisterWindow(HWND hWnd);
  static void UnRegisterWindow(HWND hWnd);
};

#endif