
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

static atButtonState s_keyState[atKC_Count];
static atButtonState s_mouseState[atMB_Count];
static atVec2I s_mousePos;
static atVec2I s_lastMousePos;
static atVec2F s_mouseVel;
static atVec2I s_lockPos = { 0,0 };
static bool s_mouseLocked = false;
static atHashMap<HWND, bool> s_windows;

bool s_mouseSet = false;

static void _UpdateMouse()
{
  if (s_mouseSet)
    atInput::OnMouseMove(s_mousePos, 0);
  if (s_mouseLocked)
    if (GetFocus())
      atInput::SetMousePos(s_lockPos);
}

static HWND _GetFocus()
{
  HWND focus = GetFocus();
  for (auto& kvp: s_windows)
    if (focus == kvp.m_key)
      return focus;
  return NULL;
}

void atInput::OnMouseMove(const atVec2I &pos, const double dt)
{
  if (!s_mouseSet)
    s_mousePos = pos;
  s_lastMousePos = s_mousePos;
  s_mousePos = pos;
  s_mouseVel = atVec2F(s_mousePos - s_lastMousePos) / dt;
  s_mouseSet = true;
}

bool atInput::Update(const bool escExit) 
{
  _UpdateMouse();
  bool res = atWindow_PumpMessage() && (!escExit || !KeyDown(atKC_Escape));
  return res;
}

void atInput::LockMouse(const bool lock, const atVec2I &pos)
{
  s_mouseLocked = lock;
  s_lockPos = pos;
}

void atInput::SetMousePos(const atVec2I &pos, const bool updateLastPos /*= true*/)
{
  atUnused(pos, updateLastPos);
  s_lastMousePos = pos;
  s_mousePos = pos; 
  POINT sc = { pos.x, pos.y };
  HWND focused = _GetFocus();
  if (focused)
    ClientToScreen(focused, &sc);
  SetCursorPos(sc.x, sc.y);
}

void atInput::OnKeyDown(const int64_t keyCode, const double dt) { s_keyState[keyCode].OnDown(dt); }
void atInput::OnKeyUp(const int64_t keyCode, const double dt) { s_keyState[keyCode].OnUp(dt); }
void atInput::OnMouseDown(const int64_t mb, const double dt) { s_mouseState[mb].OnDown(dt); }
void atInput::OnMouseUp(const int64_t mb, const double dt) { s_mouseState[mb].OnUp(dt); }
const atButtonState &atInput::GetKey(int64_t keyCode) { return s_keyState[keyCode]; }
bool atInput::KeyDown(const int64_t key) { return s_keyState[key].IsDown(); }
bool atInput::KeyUp(const int64_t key) { return s_keyState[key].IsUp(); }
bool atInput::KeyPressed(const int64_t key) { return s_keyState[key].IsPressed(); }
bool atInput::KeyReleased(const int64_t key) { return s_keyState[key].IsReleased(); }
double atInput::KeyDownTime(const int64_t key) { return s_keyState[key].TimeDown(); }
double atInput::KeyUpTime(const int64_t key) { return s_keyState[key].TimeUp(); }
bool atInput::MouseDown(const int64_t mb) { return s_mouseState[mb].IsDown(); }
bool atInput::MouseUp(const int64_t mb) { return s_mouseState[mb].IsUp(); }
bool atInput::MousePressed(const int64_t mb) { return s_mouseState[mb].IsPressed(); }
bool atInput::MouseReleased(const int64_t mb) { return s_mouseState[mb].IsReleased(); }
double atInput::MouseDownTime(const int64_t mb) { return s_mouseState[mb].TimeDown(); }
double atInput::MouseUpTime(const int64_t mb) { return s_mouseState[mb].TimeUp(); }
bool atInput::LeftMouseUp() { return MouseUp(atMB_Left); }
bool atInput::LeftMouseDown() { return MouseDown(atMB_Left); }
bool atInput::LeftMousePressed() { return MousePressed(atMB_Left); }
bool atInput::LeftMouseReleased() { return MouseReleased(atMB_Left); }
double atInput::LeftMouseUpTime() { return MouseUpTime(atMB_Left); }
double atInput::LeftMouseDownTime() {  return MouseDownTime(atMB_Left); }
double atInput::RightMouseUpTime() { return MouseUpTime(atMB_Right); }
double atInput::RightMouseDownTime() { return MouseDownTime(atMB_Right); }
bool atInput::RightMouseDown() { return MouseDown(atMB_Right); }
bool atInput::RightMouseUp() { return MouseUp(atMB_Right); }
bool atInput::RightMousePressed() { return MousePressed(atMB_Right); }
bool atInput::RightMouseReleased() { return MouseReleased(atMB_Right); }
double atInput::MiddleMouseUpTime() { return MouseUpTime(atMB_Middle); }
double atInput::MiddleMouseDownTime() { return MouseDownTime(atMB_Middle); }
bool atInput::MiddleMouseDown() { return MouseDown(atMB_Middle); }
bool atInput::MiddleMouseUp() { return MouseUp(atMB_Middle); }
bool atInput::MiddleMousePressed() { return MousePressed(atMB_Middle); }
bool atInput::MiddleMouseReleased() { return MouseReleased(atMB_Middle); }
bool atInput::IsMouseLocked() { return s_mouseLocked; }
atVec2I atInput::MouseDelta() { return s_mousePos - s_lastMousePos; }
const atVec2I& atInput::MousePos() { return s_mousePos; }
const atVec2F& atInput::MouseVelocity() { return s_mouseVel; }
atVec2F atInput::MouseDirection() { return s_mouseVel.Normalize(); }
bool atInput::MouseMoved() { return s_mousePos != s_lastMousePos; }

void atInput::RegisterWindow(HWND hWnd) 
{ 
  s_windows.TryAdd(hWnd);
}

void atInput::UnRegisterWindow(HWND hWnd) 
{ 
  if (s_windows.Contains(hWnd)) 
    s_windows.Remove(hWnd); 
}
