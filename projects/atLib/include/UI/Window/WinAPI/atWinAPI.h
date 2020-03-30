
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

#ifndef atWinAPI_h__
#define atWinAPI_h__

#include <windows.h>
#include "atColor.h"
#include "atString.h"

// TODO: Software window WINAPI https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/nf-wingdi-createdibsection

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

  bool Create();
  void Destroy();
  void SetTitle();
  void OnResize();
  void SetWindowRect();
  void SetWindowed();
  void SetVisible();

  void Maximize();
  void Minimize();
  void Restore();

  void SetParent(HWND hParent);
  void SetCursor(HCURSOR hParent);
  void SetMenu(HMENU hParent);
  void SetIcon(HICON hParent);
  void SetCallback(LRESULT(__stdcall *wndProc)(HWND, UINT, WPARAM, LPARAM));

  void Clear(atCol color);
  void Swap();

  HWND Handle() const;
  const atVector<atCol>& Pixels();
  
protected:
  bool WINRegister();
  bool WINCreate();
  bool UpdatePixels();
  void LoadDefaultResources();

  // Hi-level window
  atVector<atCol> m_pixels;
  atVector<atString> m_droppedFiles;
  atWindow *m_pWindow;

  // WinAPI junk
  atString m_wndCls = "atDefaultWndCls";
  HWND m_hWnd = NULL;
  HWND m_hParent = NULL;
  HMENU m_hMenu = NULL;
  HICON m_hIcon = NULL;
  HCURSOR m_hCursor = NULL;
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

#endif // atWinAPI_h__
