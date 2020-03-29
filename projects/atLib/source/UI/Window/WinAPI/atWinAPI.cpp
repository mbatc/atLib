
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

#ifdef atPLATFORM_WIN32

#include "atWinAPI.h"
#include "atInput.h"
#include "atImGui.h"
#include "atHashMap.h"

#include <windowsx.h>
#include <hidusage.h>
#include <gdiplus.h>
#include <time.h>

#pragma comment(lib, "gdiplus")

static MSG s_msg;
static int64_t s_lastClock;
static atHashMap<int64_t, atWindow*> _windows;
static int64_t s_wndClsCounter = 0;

atWindowStyle _MakeStyle(int64_t wndStyle);

// WinAPI helpers

LRESULT __stdcall atWinAPI::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
  case WM_MOUSEMOVE: break;
  case WM_SIZE: case WM_MOVE:
  {
    atWindow **ppTarget = _windows.TryGet((int64_t)hWnd);
    if (ppTarget)
      (*ppTarget)->OnResize();
  }
  break;
  case WM_DROPFILES:
  {
    atWindow **ppTarget = _windows.TryGet((int64_t)hWnd);
    HDROP hDrop = (HDROP)wParam;
    if (ppTarget)
    {
      int64_t fileCount = (int64_t)DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
      atVector<char> fn;
      for (int64_t i = 0; i < fileCount; ++i)
      {
        int64_t bufSize = DragQueryFile(hDrop, (UINT)i, nullptr, 0);
        fn.resize(bufSize + 1, 0);
        memset(fn.data(), 0, fn.size());

        DragQueryFile(hDrop, (UINT)i, fn.data(), (UINT)fn.size());
        (*ppTarget)->AddDroppedFile(fn.data());
      }
    }
    DragFinish(hDrop);
  }
  break;
  default: return DefWindowProc(hWnd, msg, wParam, lParam);
  }
  atInput::SetDT(dt);
  return 0;
}

static void _RegisterInputDevices(HWND hWnd)
{
  RAWINPUTDEVICE Rid;
  Rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
  Rid.usUsage = HID_USAGE_GENERIC_MOUSE;
  Rid.dwFlags = RIDEV_INPUTSINK;
  Rid.hwndTarget = hWnd;
  atAssert(!!RegisterRawInputDevices(&Rid, 1, sizeof(RAWINPUTDEVICE)), "Failed to register input device.");
}

bool atWinAPI::PumpMessage(atWindow *pWindow)
{
  while (PeekMessage(&s_msg, pWindow ? pWindow->Handle() : 0, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&s_msg);
    DispatchMessage(&s_msg);
    if (s_msg.message == WM_QUIT)
      return false;
  }
  s_lastClock = clock();
  return true;
}

int atWinAPI::GetResult() { return (int)s_msg.wParam; }

void atWinAPI::RegisterWindow(HWND hWnd, atWindow *pWindow)
{
  UnRegisterWindow(hWnd);
  _RegisterInputDevices(hWnd);
  _windows.Add((int64_t)hWnd, pWindow);
}

void atWinAPI::UnRegisterWindow(HWND hWnd) { _windows.Remove((int64_t)hWnd); }

atWindow* atWinAPI::GetWindow(HWND hWnd)
{
  atWindow **ppWnd = _windows.TryGet((int64_t)hWnd);
  return ppWnd ? *ppWnd : nullptr;
}

// atWin32Window

static int64_t s_nWindows = 0;
static ULONG_PTR s_gdiToken;

atWin32Window::atWin32Window(atWindow *pWindow)
{
  m_pWindow = pWindow;

  if (++s_nWindows != 1)
    return;

  m_wndCls += s_wndClsCounter;

  // Initialise Gdiplus on first windows creation
  Gdiplus::GdiplusStartupInput _tmp;
  Gdiplus::GdiplusStartup(&s_gdiToken, &_tmp, NULL);
}

atWin32Window::~atWin32Window()
{
  if (--s_nWindows == 0)
    Gdiplus::GdiplusShutdown(s_gdiToken);
}

bool atWin32Window::Create()
{
  if (!WINRegister())
    return false;
  if (!WINCreate())
    return false;
  UpdateWindow(m_hWnd);
  SetVisible();
  atWinAPI::RegisterWindow(m_hWnd, m_pWindow);
  return true;
}

void atWin32Window::Destroy()
{
  if (m_hWnd)
  {
    atWinAPI::UnRegisterWindow(m_hWnd);
    DestroyWindow(m_hWnd);
    m_hWnd = NULL;
    UnregisterClass(m_wndCls.c_str(), GetModuleHandle(NULL));
  }

  if(m_hIcon) DestroyIcon(m_hIcon);
  if(m_hCursor) DestroyCursor(m_hCursor);
}

void atWin32Window::OnResize()
{
  RECT clientRect, wndRect;
  GetClientRect(m_hWnd, &clientRect);
  GetWindowRect(m_hWnd, &wndRect);
  m_pWindow->m_clientSize = atVec2I(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
  m_pWindow->m_size = atVec2I(wndRect.right - wndRect.left, wndRect.bottom - wndRect.top);
  m_pWindow->m_pos = atVec2I(wndRect.left, wndRect.top);
  UpdatePixels();
}

void atWin32Window::SetWindowRect(const atVec4I &rect)
{
  if (!m_hWnd)
    return;
  SetWindowPos(m_hWnd, NULL,
    rect.x,
    rect.y,
    rect.z,
    rect.w, 0);
  UpdatePixels();
}

void atWin32Window::SetWindowed(const bool &windowed)
{
  bool fullscreen = !windowed;
  
  if (!m_windowedState.wasFullscreen)
  {
    m_windowedState.maximized = IsZoomed(m_hWnd) == TRUE;

    if (m_windowedState.maximized)
      SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
    m_windowedState.style = GetWindowLong(m_hWnd, GWL_STYLE);
    m_windowedState.exStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
    GetWindowRect(m_hWnd, &m_windowedState.rect);
  }

  m_windowedState.wasFullscreen = fullscreen;

  if (fullscreen)
  {
    // Set new window style and size.
    SetWindowLong(m_hWnd, GWL_STYLE, (LONG)m_windowedState.style & ~(WS_CAPTION | WS_THICKFRAME));
    SetWindowLong(m_hWnd, GWL_EXSTYLE, (LONG)m_windowedState.exStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));

    // On expand, if we're given a window_rect, grow to it, otherwise do not resize.
    MONITORINFO monitor_info;
    monitor_info.cbSize = sizeof(monitor_info);
    GetMonitorInfo(MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST), &monitor_info);
    RECT windowRect(monitor_info.rcMonitor);
    SetWindowPos(m_hWnd, NULL, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
  }
  else
  {
    // Restore window sizes
    SetWindowLong(m_hWnd, GWL_STYLE, (LONG)m_windowedState.style);
    SetWindowLong(m_hWnd, GWL_EXSTYLE, (LONG)m_windowedState.exStyle);

    // Restore window rect
    RECT windowRect(m_windowedState.rect);
    SetWindowPos(m_hWnd, NULL, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

    // Restore maximized state
    if (m_windowedState.maximized)
      SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
  }
}

void atWin32Window::SetVisible(const bool &visible) { ShowWindow(m_hWnd, visible ? SW_SHOW : SW_HIDE); }
void atWin32Window::Maximize() { ShowWindow(m_hWnd, SW_MAXIMIZE); }
void atWin32Window::Minimize() { ShowWindow(m_hWnd, SW_MINIMIZE); }
void atWin32Window::Restore() { ShowWindow(m_hWnd, SW_RESTORE); }

bool atWin32Window::WINRegister()
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

bool atWin32Window::WINCreate()
{
  HINSTANCE hInstance = GetModuleHandle(NULL);
  atVec2I pos = m_pWindow->Position();
  atVec2I size = m_pWindow->Size();
  int64_t style = m_pWindow->m_style;
  RECT rect = { pos.x, pos.y, pos.x + size.x, pos.y + size.y };
  AdjustWindowRect(&rect, (DWORD)style, false);
  m_pWindow->m_clientSize = size;
  m_pWindow->m_size = { rect.right - rect.left, rect.bottom - rect.top };
  size = m_pWindow->m_size;
  m_hWnd = CreateWindow(m_wndCls.c_str(), m_pWindow->m_title.c_str(), (DWORD)style, pos.x, pos.y, size.x, size.y, m_hParent, m_hMenu, hInstance, NULL);
  if (!m_hWnd)
    return false;

  // Enabled drag and drop
  DragAcceptFiles(m_hWnd, TRUE);
  
  return UpdatePixels();
}

void atWin32Window::LoadDefaultResources()
{
  if (!m_hIcon)
    m_hIcon = LoadIcon(NULL, IDI_APPLICATION);
  if (!m_hCursor)
    m_hCursor = LoadCursor(NULL, IDC_ARROW);
}

bool atWin32Window::UpdatePixels()
{
  m_pixels.resize(m_pWindow->m_clientSize.x * m_pWindow->m_clientSize.y);
  return true;
}

void atWin32Window::Swap()
{
  HDC hDC = GetDC(m_hWnd);
  const atVector<atCol> &pixels = m_pWindow->PixelsV();
  atVec2I size = m_pWindow->Size();

  if (pixels.size() != 0)
  {
    // Flip RGBA to BGRA
    static atVector<atCol> pixelsBGRA;
    pixelsBGRA = pixels;
    for (atCol &c : pixelsBGRA) c = atColor::RGBAtoBGRA(c);

    // Create Bitmap Info
    BITMAPINFO info = { 0 };
    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biBitCount = 32;
    info.bmiHeader.biCompression = BI_RGB;
    info.bmiHeader.biHeight = size.y;
    info.bmiHeader.biWidth = size.x;
    info.bmiHeader.biPlanes = 1;

    // Draw Bitmap
    StretchDIBits(hDC, 0, 0, size.x, size.y, 0, 0, size.x, size.y, pixelsBGRA.data(), &info, DIB_RGB_COLORS, SRCCOPY);
  }

  ReleaseDC(m_hWnd, hDC);
}

void atWin32Window::Clear(const atCol &color) { for (atCol &c : m_pixels) memcpy(&c, &color, sizeof(atCol)); }
void atWin32Window::SetCallback(LRESULT(__stdcall *wndProc)(HWND, UINT, WPARAM, LPARAM)) { m_wndProc = wndProc; }
void atWin32Window::SetTitle() { SetWindowText(m_hWnd, m_pWindow->m_title.c_str()); }
void atWin32Window::SetCursor(HCURSOR hCursor) { m_hCursor = hCursor; }
void atWin32Window::SetParent(HWND hParent) { m_hParent = hParent; }
const atVector<atCol>& atWin32Window::Pixels() { return m_pixels; }
void atWin32Window::SetIcon(HICON hIcon) { m_hIcon = hIcon; }
void atWin32Window::SetMenu(HMENU hMenu) { hMenu = hMenu; }
HWND atWin32Window::Handle() const { return m_hWnd; }

atWindowStyle atWin32Window::GetStyle() const { return _MakeStyle(GetWindowLong(m_hWnd, GWL_STYLE)); }

atString atWin32Window::GetTitle() const
{
  int64_t titleLen = GetWindowTextLength(m_hWnd) + 1;
  atVector<char> titleBuffer;
  titleBuffer.resize(titleLen, 0);
  GetWindowText(m_hWnd, titleBuffer.data(), titleLen);
}

atVec2I atWin32Window::GetSize() const
{
  RECT r;
  GetWindowRect(&r);
  return atVec2I(r.right - r.left, r.bottom - r.top);
}

atVec2I atWin32Window::GetPos() const
{
  RECT r;
  GetWindowRect(&r);
  return atVec2I(r.left, r.top);
}

bool atWin32Window::IsMaximized() const { return IsZoomed(m_hWnd); }
bool atWin32Window::IsMinimized() const { return IsIconic(m_hWnd) }
bool atWin32Window::IsWindowed() const { return !m_windowedState.wasFullscreen; }
bool atWin32Window::IsVisible() const { return IsWindowVisible(m_hWnd); }

atWindowStyle _MakeStyle(int64_t wndStyle)
{
  return atWindowStyle(0);
}

#endif