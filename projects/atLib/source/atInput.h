
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

#include <Windows.h>
#include "atButtonState.h"
#include "atMath.h"

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
  atKC_V = 'V',
  atKC_W = 'W',
  atKC_X = 'X',
  atKC_Y = 'Y',
  atKC_Z = 'Z',
  atKC_Hyphen = '-',
  atKC_Equals = '=',
  atKC_Apostraphe = '\'',
  atKC_OpenSqrBracket = '[',
  atKC_CloseSqrBracket = ']',
  atKC_Period = '.',
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
  atKC_LOEM = VK_LWIN,
  atKC_ROEM = VK_RWIN,

  atKC_Count = 256
};

enum atMouseButton
{
  atMB_Left,
  atMB_Right,
  atMB_Middle,
  atMB_Count = 3
};

class atWindow;

class atInput
{
  friend atWindow;
public:
  static bool Update(const bool escExit = true);

  static void OnKeyDown(const int64_t keyCode, const double dt);
  static void OnKeyUp(const int64_t keyCode, const double dt);
  static void OnMouseDown(const int64_t mb, const double dt);
  static void OnMouseUp(const int64_t mb, const double dt);
  static void OnMouseMove(const atVec2I &pos, const double dt);

  static const atButtonState &GetKey(int64_t keyCode);
  static bool KeyDown(const int64_t key);
  static bool KeyUp(const int64_t key);
  static bool KeyPressed(const int64_t key);
  static bool KeyReleased(const int64_t key);
  static double KeyDownTime(const int64_t key);
  static double KeyUpTime(const int64_t key);

  static bool MouseDown(const int64_t mb);
  static bool MouseUp(const int64_t mb);
  static bool MousePressed(const int64_t mb);
  static bool MouseReleased(const int64_t mb);
  static double MouseDownTime(const int64_t mb);
  static double MouseUpTime(const int64_t mb);

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
  static const atVec2I& MousePos();
  static const atVec2F& MouseVelocity();
  static void SetMousePos(const atVec2I& pos, const bool updateLastPos = true);

  static bool MouseMoved();

  static void SetDT(const double dt);
protected:
  static void RegisterWindow(HWND hWnd);
  static void UnRegisterWindow(HWND hWnd);
};

#endif