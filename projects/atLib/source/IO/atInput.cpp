
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

#include "atInput.h"
#include "atWindow.h"
#include "atHashMap.h"
#include "atIterator.h"

static atButtonState s_buttonState[atKC_Count + atKC_MB_Count];

static atVec2F _mousePos;
static atVec2I _lastMousePos;
static atVec2F _mouseVel;
static atVec2I _lockPos = { 0,0 };
static double _dt = 0.0;
static bool _mouseLocked = false;
static atHashMap<int64_t, bool> _windows;
static atVec2F _mouseScroll;
static HCURSOR _hLastCursor = NULL;

bool _mouseSet = false;

static void _UpdateMouse()
{
  POINT p;
  GetCursorPos(&p);
  HWND hWnd = GetFocus();
  if (atWinAPI::GetWindow(hWnd))
  {
    POINT tl = { 0, 0 };
    ClientToScreen(hWnd, &tl);
    atInput::OnMouseMove({ p.x - tl.x, p.y - tl.y }, _dt);
  }
}

static void _UpdateButtons()
{
  for (atButtonState &bt : atIterate(s_buttonState, atKC_Count + atKC_MB_Count))
    bt.Update(_dt);
}

static HWND _GetFocus()
{
  HWND focus = GetFocus();
  if (atWinAPI::GetWindow(focus))
    return focus;
  return NULL;
}

void atInput::OnMouseMove(const atVec2I &pos, const double dt)
{
  if (!_mouseSet)
    _mousePos = pos;

  _lastMousePos = _mousePos;
  _mousePos = pos;
  _mouseVel = atVec2F(_mousePos - _lastMousePos) / dt;
  _mouseSet = true;
}

bool atInput::Update(const bool escExit, atWindow *pWindow)
{
  _UpdateMouse();
  _UpdateButtons();
  bool res = atWindow::PumpMessage(pWindow) && (!escExit || !ButtonDown(atKC_Escape));
  return res;
}

void atInput::LockMouse(const bool lock, const atVec2I &pos)
{
  _mouseLocked = lock;
  _lockPos = pos;
}

void atInput::SetMousePos(const atVec2I &pos, const bool updateLastPos /*= true*/)
{
  atUnused(pos, updateLastPos);
  _lastMousePos = pos;
  _mousePos = pos; 
  POINT sc = { pos.x, pos.y };
  HWND focused = _GetFocus();
  if (focused)
    ClientToScreen(focused, &sc);
  SetCursorPos(sc.x, sc.y);
}

void atInput::RegisterWindow(HWND hWnd)
{
  _windows.TryAdd((int64_t)hWnd);
}

void atInput::UnRegisterWindow(HWND hWnd)
{
  if (_windows.Contains((int64_t)hWnd))
    _windows.Remove((int64_t)hWnd);
}

const atButtonState &atInput::GetButton(int64_t keyCode) { return s_buttonState[keyCode]; }
void atInput::OnButtonDown(const int64_t keyCode, const double dt) { s_buttonState[keyCode].OnDown(dt); }
void atInput::OnButtonUp(const int64_t keyCode, const double dt) { s_buttonState[keyCode].OnUp(dt); }
bool atInput::ButtonDown(const int64_t key) { return s_buttonState[key].IsDown(); }
bool atInput::ButtonUp(const int64_t key) { return s_buttonState[key].IsUp(); }
bool atInput::ButtonPressed(const int64_t key) { return s_buttonState[key].IsPressed(); }
bool atInput::ButtonReleased(const int64_t key) { return s_buttonState[key].IsReleased(); }
double atInput::ButtonDownTime(const int64_t key) { return s_buttonState[key].TimeDown(); }
double atInput::ButtonUpTime(const int64_t key) { return s_buttonState[key].TimeUp(); }
bool atInput::LeftMouseUp() { return ButtonUp(atKC_MB_Left); }
bool atInput::LeftMouseDown() { return ButtonDown(atKC_MB_Left); }
bool atInput::LeftMousePressed() { return ButtonPressed(atKC_MB_Left); }
bool atInput::LeftMouseReleased() { return ButtonReleased(atKC_MB_Left); }
double atInput::LeftMouseUpTime() { return ButtonUpTime(atKC_MB_Left); }
double atInput::LeftMouseDownTime() {  return ButtonDownTime(atKC_MB_Left); }
double atInput::RightMouseUpTime() { return ButtonUpTime(atKC_MB_Right); }
double atInput::RightMouseDownTime() { return ButtonDownTime(atKC_MB_Right); }
bool atInput::RightMouseDown() { return ButtonDown(atKC_MB_Right); }
bool atInput::RightMouseUp() { return ButtonUp(atKC_MB_Right); }
bool atInput::RightMousePressed() { return ButtonPressed(atKC_MB_Right); }
bool atInput::RightMouseReleased() { return ButtonReleased(atKC_MB_Right); }
double atInput::MiddleMouseUpTime() { return ButtonUpTime(atKC_MB_Middle); }
double atInput::MiddleMouseDownTime() { return ButtonDownTime(atKC_MB_Middle); }
bool atInput::MiddleMouseDown() { return ButtonDown(atKC_MB_Middle); }
bool atInput::MiddleMouseUp() { return ButtonUp(atKC_MB_Middle); }
bool atInput::MiddleMousePressed() { return ButtonPressed(atKC_MB_Middle); }
bool atInput::MiddleMouseReleased() { return ButtonReleased(atKC_MB_Middle); }
bool atInput::IsMouseLocked() { return _mouseLocked; }
atVec2I atInput::MouseDelta() { return _mousePos - _lastMousePos; }
void atInput::OnMouseWheelH(const float scroll) { _mousePos.x += scroll; }
const float atInput::MouseScrollX() { return MouseScroll().x; }
const float atInput::MouseScrollY() { return MouseScroll().y; }
const atVec2F& atInput::MouseScroll() { return _mouseScroll; }
const atVec2F& atInput::MousePos() { return _mousePos; }
const atVec2F& atInput::MouseVelocity() { return _mouseVel; }
atVec2F atInput::MouseDirection() { return _mouseVel.Normalize(); }
void atInput::OnMouseWheel(const float scroll) { _mouseScroll.y += scroll; }
bool atInput::MouseMoved() { return _mousePos != _lastMousePos; }
void atInput::SetDT(const double dt) { _dt = dt; }

// Giant switch statement coming up

atString atInput::ToString(const int64_t code)
{
  switch (code)
  {
  case atKC_0:                  return "_0";
  case atKC_1:                  return "_1";
  case atKC_2:                  return "_2";
  case atKC_3:                  return "_3";
  case atKC_4:                  return "_4";
  case atKC_5:                  return "_5";
  case atKC_6:                  return "_6";
  case atKC_7:                  return "_7";
  case atKC_8:                  return "_8";
  case atKC_9:                  return "_9";
  case atKC_A:                  return "A";
  case atKC_B:                  return "B";
  case atKC_C:                  return "C";
  case atKC_D:                  return "D";
  case atKC_E:                  return "E";
  case atKC_F:                  return "F";
  case atKC_G:                  return "G";
  case atKC_H:                  return "H";
  case atKC_I:                  return "I";
  case atKC_J:                  return "J";
  case atKC_K:                  return "K";
  case atKC_L:                  return "L";
  case atKC_M:                  return "M";
  case atKC_N:                  return "N";
  case atKC_O:                  return "O";
  case atKC_P:                  return "P";
  case atKC_Q:                  return "Q";
  case atKC_R:                  return "R";
  case atKC_S:                  return "S";
  case atKC_T:                  return "T";
  case atKC_U:                  return "U";
  case atKC_V:                  return "V";
  case atKC_W:                  return "W";
  case atKC_X:                  return "X";
  case atKC_Y:                  return "Y";
  case atKC_Z:                  return "Z";
  case atKC_Equals:             return "Equals";
  case atKC_Apostraphe:         return "Apostraphe";
  case atKC_OpenSqrBracket:     return "OpenSqrBracket";
  case atKC_CloseSqrBracket:    return "CloseSqrBracket";
  case atKC_Period:             return "Period";
  case atKC_Comma:              return "Comma";
  case atKC_ForwardSlash:       return "ForwardSlash";
  case atKC_BackSlash:          return "BackSlash";
  case atKC_Tab:                return "Tab";
  case atKC_CapsLock:           return "CapsLock";
  case atKC_Shift:              return "Shift";
  case atKC_LShift:             return "LShift";
  case atKC_RShift:             return "RShift";
  case atKC_Control:            return "Control";
  case atKC_LControl:           return "LControl";
  case atKC_RControl:           return "RControl";
  case atKC_Return:             return "Return";
  case atKC_Backspace:          return "Backspace";
  case atKC_LAlt:               return "LAlt";
  case atKC_RAlt:               return "RAlt";
  case atKC_Space:              return "Space";
  case atKC_Delete:             return "Delete";
  case atKC_Home:               return "Home";
  case atKC_Insert:             return "Insert";
  case atKC_PageDown:           return "PageDown";
  case atKC_PageUp:             return "PageUp";
  case atKC_End:                return "End";
  case atKC_ScrollLock:         return "ScrollLock";
  case atKC_PauseBreak:         return "PauseBreak";
  case atKC_PrintScreen:        return "PrintScreen";
  case atKC_NumLock:            return "NumLock";
  case atKC_Left:               return "Left";
  case atKC_Right:              return "Right";
  case atKC_Up:                 return "Up";
  case atKC_Down:               return "Down";
  case atKC_Escape:             return "Escape";
  case atKC_F1:                 return "F1";
  case atKC_F2:                 return "F2";
  case atKC_F3:                 return "F3";
  case atKC_F4:                 return "F4";
  case atKC_F5:                 return "F5";
  case atKC_F6:                 return "F6";
  case atKC_F7:                 return "F7";
  case atKC_F8:                 return "F8";
  case atKC_F9:                 return "F9";
  case atKC_F10:                return "F10";
  case atKC_F11:                return "F11";
  case atKC_F12:                return "F12";
  case atKC_Menu:               return "Menu";
  case atKC_MB_Left:            return "LeftMouse";
  case atKC_MB_Right:           return "RightMouse";
  case atKC_MB_Middle:          return "MiddleMouse";
  }
  return atString();
}
