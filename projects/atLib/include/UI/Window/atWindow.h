
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

#ifndef _atWindow_h__
#define _atWindow_h__

#include "atWinAPI.h"
#include "atMath.h"

class atGraphics;
class atRenderState;
class atWin32Window;
class atWinAPI;

class atWindow
{
  friend atRenderState;
  friend atWin32Window;
  friend atGraphics;
  friend atWinAPI;

public:
  static bool PumpMessage(atWindow *pWindow);
  static int GetResult();

  atWindow(const atString &title = "Default Window", const atVec2I &size = atVec2I(800, 600), const atVec2I &pos = atVec2I(0, 0), const bool windowed = true, const bool &visible = true, const int64_t style = WS_OVERLAPPEDWINDOW);
  ~atWindow();

  void Clear(const atCol color = 0xFF000000);
  void Clear(const atVec4F &color);

  void Swap();

  void SetTitle(const atString &title);
  void SetPos(const atVec2I &pos);
  void SetSize(const atVec2I &size);
  void SetStyle(const int64_t style);
  void SetWindowed(const bool windowed);
  void SetVisible(const bool &visible);
  void Maximize();
  void Minimize();
  void Restore();

  const atVec2I& Size() const;
  const int32_t& Width() const;
  const int32_t& Height() const;

  const atVec2I& Position() const;
  const int32_t& GetX() const;
  const int32_t& GetY() const;

  bool IsMaximized() const;
  bool IsMinimized() const;
  bool IsRestored() const;
  bool IsWindowed() const;
  bool IsVisible() const;
  void SetMenu(HMENU hMenu);
  void SetIcon(HICON hIcon);
  void SetCursor(HCURSOR hCursor);
  void SetParent(const atWindow &window);
  void SetWndProc(LRESULT(__stdcall *wndProc)(HWND, UINT, WPARAM, LPARAM));

  HWND Handle() const;

  bool MakeWindow();
  void Destroy();
  void OnResize();

  atCol* Pixels();

  atVector<atString> DroppedFiles();
  
  bool PumpMessage();

protected:
  void SetHardwareCtx(atGraphics *pGfx);
  void AddDroppedFile(const atString &file);

  const atVector<atCol>& PixelsV();
  bool m_windowed = true;
  bool m_visible = true;
  bool m_maximized = false;
  bool m_minimized = false;
  
  atString m_title = "Main Window";

  atVec2I m_clientSize = atVec2I(800, 600);
  atVec2I m_size = atVec2I(800, 600);
  atVec2I m_pos = atVec2I(0, 0);

  int64_t m_style = WS_OVERLAPPEDWINDOW;

  atVector<atString> m_droppedFiles;

  atGraphics *m_pGfx = nullptr;

#if defined _WIN64 || defined _WIN64
  // WINAPI  
  atWin32Window m_sysWindow;
#elif LINUX
  // Add linux m_sysWindow
#endif
};

#endif // _atWindow_h__