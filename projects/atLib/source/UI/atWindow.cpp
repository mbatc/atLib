
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

#include "atHashMap.h"
#include "atWindow.h"
#include "atImGui.h"
#include "atInput.h"
#include <Windowsx.h>
#include <time.h>

static MSG s_msg;
static int64_t s_lastClock = 0;
static atHashMap<int64_t, atWindow*> _windows;

bool atWindow_PumpMessage()
{
  while (PeekMessage(&s_msg, NULL, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&s_msg);
    DispatchMessage(&s_msg);
    if (s_msg.message == WM_QUIT)
      return false;
  }
  s_lastClock = clock();
  return true;
}

int atWindow_GetResult() { return (int)s_msg.wParam; }


LRESULT __stdcall atLibDefWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  const double dt = (double)(clock() - s_lastClock) / (double)CLOCKS_PER_SEC;
  if (!atImGui::ProcessMessage(hWnd, msg, wParam, lParam))
  {
    // Messages blocked by ImGui windows
    switch (msg)
    {
    case WM_KEYDOWN: atInput::OnButtonDown(wParam, dt); break;
    case WM_LBUTTONDOWN: atInput::OnButtonDown(atKC_MB_Left, dt); break;
    case WM_RBUTTONDOWN: atInput::OnButtonDown(atKC_MB_Right, dt); break;
    case WM_MBUTTONDOWN: atInput::OnButtonDown(atKC_MB_Middle, dt); break;
    case WM_MOUSEWHEEL: atInput::OnMouseWheel((float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA); break;
    case WM_MOUSEHWHEEL: atInput::OnMouseWheelH((float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA); break;
    }
  }

  switch (msg)
  {
  case WM_CLOSE:  PostQuitMessage(0); break;
  case WM_DESTROY: PostQuitMessage(0); break; 
  case WM_KEYUP: atInput::OnButtonUp(wParam, dt); break;
  case WM_LBUTTONUP: atInput::OnButtonUp(atKC_MB_Left, dt); break;
  case WM_RBUTTONUP: atInput::OnButtonUp(atKC_MB_Right, dt); break;
  case WM_MBUTTONUP: atInput::OnButtonUp(atKC_MB_Middle, dt); break;
  case WM_MOUSEMOVE: atInput::OnMouseMove({ (GET_X_LPARAM(lParam)), (GET_Y_LPARAM(lParam)) }, dt); break;
  case WM_SIZE: case WM_MOVE:
  {
    atWindow **ppTarget = _windows.TryGet((int64_t)hWnd);
    if (ppTarget)
      (*ppTarget)->OnResize();
  }
  break;
  default: return DefWindowProc(hWnd, msg, wParam, lParam);
  }
  atInput::SetDT(dt);
  return 0;
}

static bool _BlitWindow(HWND hWnd, HBITMAP hBitmap, const atWindow::Pixel *pPixels, const int64_t width, const int64_t height)
{
  if (!hBitmap || !pPixels || width == 0 || height == 0)
    return false;

  SetBitmapBits(hBitmap, (DWORD)(width * height), pPixels);
  SendMessage(hWnd, WM_PAINT, (WPARAM)hBitmap, 0);
  return true;
}

atWindow::atWindow(const atString &title, const atVec2I &size, const bool hardware, const atVec2I &pos, const bool windowed, const int64_t style)
  : m_dxTarget(this, size)
  , m_hardware(hardware)
{
  SetTitle(title);
  SetSize(size);
  SetPos(pos);
  SetStyle(style);
  SetWindowed(windowed);
  MakeWindow();
}

atWindow::~atWindow()
{
  Destroy();
  DestroyIcon(m_hIcon);
  DestroyCursor(m_hCursor);
}

void atWindow::Clear(const Pixel color)
{
  for (int64_t i = 0; i < m_pixels.size(); ++i)
    m_pixels[i] = color;
  Clear(atVec4F((float)color.r / 255.f, (float)color.g / 255.f, (float)color.b / 255.f, 1.0f));
}

void atWindow::Clear(const atCol color) { m_dxTarget.Clear(atColor::UnPack<float>(color)); }

void atWindow::Clear(const atVec4F &color) { m_dxTarget.Clear(color); }

void atWindow::Clear(const char r, const char g, const char b) { Clear(Pixel(b, g, r)); }

void atWindow::Swap()
{
  // Swap software window
  // if (_BlitWindow(m_hWnd, m_hBitmap, m_pixels.data(), m_size.x, m_size.y))
  //   return; // If successful return

  // If not swap hardware window
  m_dxTarget.Swap();
}

void atWindow::SetTitle(const atString &title)
{
  m_title = title;
  if(m_hWnd)
    SetWindowText(m_hWnd, title.c_str());
}

void atWindow::SetPos(const atVec2I &pos)
{
  m_pos = pos;
  SetWindowRect();
}

void atWindow::SetSize(const atVec2I &size)
{
  m_size = size;
  SetWindowRect();
}

bool atWindow::MakeWindow()
{
  if (!WINRegister())
    return false;
  if (!WINCreate())
    return false;
  UpdateWindow(m_hWnd); 
  ShowWindow(m_hWnd, SW_SHOWDEFAULT);
  atInput::RegisterWindow(m_hWnd);
  _windows.Add((int64_t)m_hWnd, this);
  return true;
}

bool atWindow::WINRegister()
{
  LoadDefaultResources();
  HINSTANCE hInstance = GetModuleHandle(NULL);
  WNDCLASSEX wce;
  wce.cbSize = sizeof(WNDCLASSEX);
  wce.style = 0;
  wce.lpfnWndProc = m_wndProc;
  wce.cbClsExtra = 0;
  wce.cbWndExtra = 0;
  wce.hInstance = hInstance;
  wce.hIcon = m_hIcon;
  wce.hCursor = m_hCursor;
  wce.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wce.lpszMenuName = NULL;
  wce.lpszClassName = m_wndCls.c_str();
  wce.hIconSm = m_hIcon;
  return RegisterClassEx(&wce) != 0;
}

bool atWindow::WINCreate()
{
  HINSTANCE hInstance = GetModuleHandle(NULL);
  RECT rect = { m_pos.x, m_pos.y, m_pos.x + m_size.x, m_pos.y + m_size.y };
  AdjustWindowRect(&rect, (DWORD)m_style, false);
  m_clientSize = m_size;
  m_size = { rect.right - rect.left, rect.bottom - rect.top };
  m_hWnd = CreateWindow(m_wndCls.c_str(), m_title.c_str(), (DWORD)m_style, m_pos.x, m_pos.y, m_size.x, m_size.y, m_hParent, m_hMenu, hInstance, NULL); 
  ResizePixels();
  return m_hWnd != nullptr;
}

void atWindow::OnResize()
{
  RECT clientRect, wndRect;
  GetClientRect(m_hWnd, &clientRect);
  GetWindowRect(m_hWnd, &wndRect);
  m_clientSize = atVec2I(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
  m_size = atVec2I(wndRect.right - wndRect.left, wndRect.bottom - wndRect.top);
  m_pos = atVec2I(wndRect.left, wndRect.top);
  m_dxTarget.Resize();
}

void atWindow::SetWindowRect()
{
  if (!m_hWnd)
    return;
  SetWindowPos(m_hWnd, NULL, m_pos.x, m_pos.y, m_size.x, m_size.y, 0);
  ResizePixels();
}

void atWindow::LoadDefaultResources()
{
  if(!m_hIcon)
    m_hIcon = LoadIcon(NULL, IDI_APPLICATION);
  if (!m_hCursor)
    m_hCursor = LoadCursor(NULL, IDC_ARROW);
}

void atWindow::ResizePixels()
{
  if (!m_hWnd || m_clientSize.x * m_clientSize.y == m_pixels.size())
    return;

  if (m_hardware)
  {
    m_pixels.make_empty();
    return;
  }

  m_pixels.resize(m_clientSize.x * m_clientSize.y);
}

void atWindow::Destroy() 
{ 
  if (m_hWnd)
  {
    atInput::UnRegisterWindow(m_hWnd);
    _windows.Remove((int64_t)m_hWnd);
    DestroyWindow(m_hWnd);
    m_hWnd = NULL;
  }
  m_pixels.clear();
}

const atVec2I& atWindow::GetPos() const { return m_pos; }
const atVec2I& atWindow::Size() const { return m_clientSize; }
void atWindow::SetStyle(const int64_t style) { m_style = style; }
void atWindow::SetWindowed(const bool windowed) { m_windowed = windowed; m_dxTarget.Resize(); }
void atWindow::SetMenu(HMENU hMenu) { m_hMenu = hMenu; }
void atWindow::SetIcon(HICON hIcon) { m_hIcon = hIcon; }
void atWindow::SetCursor(HCURSOR hCursor) { m_hCursor = hCursor; }
void atWindow::SetParent(const atWindow &window) { m_hWnd = window.GetHandle(); }
void atWindow::SetWndProc(LRESULT(__stdcall *wndProc)(HWND, UINT, WPARAM, LPARAM)) { m_wndProc = wndProc; }
atVec2I atWindow::DisplaySize() { return atVec2I(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)); }
HWND atWindow::GetHandle() const { return m_hWnd; }
int32_t atWindow::Width() const { return Size().x; }
int32_t atWindow::Height() const { return Size().y; }
int32_t atWindow::GetX() const { return GetPos().x; }
int32_t atWindow::GetY() const { return GetPos().y; }
bool atWindow::IsWindowed() const { return m_windowed; }
