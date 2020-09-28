
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
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

#ifndef atWinAPI_h__
#define atWinAPI_h__

#include "atPlatform.h"

#include "atWindowDefinitions.h"
#include "atColor.h"
#include "atString.h"

#ifdef IsMinimized 
#define Win32IsMinimized IsMinimized
#endif

#ifdef IsMaximized
#define Win32IsMaximized IsMaximized
#endif

#undef IsMinimized
#undef IsMaximized

class atWindow;
class atWin32Window;

class atWinAPI
{
public:
  static LRESULT __stdcall WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  static bool PumpMessage(atWindow *pWindow = nullptr);
  static int GetResult();
  static void RegisterWindow(HWND hWnd, atWindow *pWindow);
  static void UnRegisterWindow(HWND hWnd);
  static atWindow* GetWindow(HWND hWnd);
};

class atWin32Window
{
  friend atWinAPI;

public:
  atWin32Window(atWindow *pWindow);
  ~atWin32Window();

  void Clear(const atCol &color);
  void Swap();

  bool Create(const atWindowCreateInfo &info);
  void Destroy();
  void SetTitle(const atString &title);
  void OnResize();
  
  void SetWindowRect(const atVec4I &rect);
  void SetWindowed(const bool &windowed);
  void SetVisible(const bool &visible);
  void SetStyle(const atWindowStyle &style);

  void Maximize();
  void Minimize();
  void Restore();

  void SetParent(const atSysWndHandle &hParent);
  void SetCursor(const atSysCursorHandle &hParent);
  void SetMenu(const atSysMenuHandle &hParent);
  void SetIcon(const atSysIconHandle &hParent);
  void SetCallback(const atSysWndCallback &callback);

  atWindowStyle GetStyle() const;

  atString GetTitle() const;
  atVec2I GetScreenPos(const atVec2I &pos) const;
  atVec2I GetClientSize() const;
  atVec2I GetSize() const;
  atVec2I GetPos() const;

  bool IsMaximized() const;
  bool IsMinimized() const;
  bool IsWindowed() const;
  bool IsVisible() const;

  int64_t GetDPI() const;
  int64_t GetDefaultDPI() const;

  atSysWndHandle Handle() const;
  atVector<atCol>& Pixels();
  const atVector<atCol>& Pixels() const;
  
protected:
  bool WINRegister(const atWindowCreateInfo &info);
  bool WINCreate(const atWindowCreateInfo &info);
  bool UpdatePixels();
  void LoadDefaultResources();

  // Hi-level window
  atVector<atCol> m_pixels;
  atVector<atCol> m_pixelsBGRA;
  atVector<atString> m_droppedFiles;
  atWindow *m_pWindow;

  // WinAPI junk
  atString m_wndCls = "atDefaultWndCls";
  atSysWndHandle m_hWnd = NULL;
  atSysWndHandle m_hParent = NULL;
  atSysMenuHandle m_hMenu = NULL;
  atSysIconHandle m_hIcon = NULL;
  atSysCursorHandle m_hCursor = NULL;

  LRESULT(__stdcall *m_wndProc)(HWND, UINT, WPARAM, LPARAM) = atWinAPI::WindowProc;

  // windowed state
  struct
  {
    bool wasFullscreen = false;
    bool maximized = false;
    int64_t style = 0;
    int64_t exStyle = 0;
    RECT rect;
  } m_windowedState;
};

#ifdef Win32IsMinimized
#define IsMinimized Win32IsMinimized
#endif

#ifdef Win32IsMaximized
#define IsMaximized Win32IsMaximized
#endif

#endif // atWinAPI_h__
