
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

#ifndef _atWindow_h__
#define _atWindow_h__

#include "atPlatform.h"

#ifdef atPLATFORM_WIN32
#include "atWinAPI.h"
#elif atPLATFORM_LINUX
#include "atXLib.h"
#endif

#include "atMath.h"

class atGraphics;
class atRenderState;
class atWin32Window;
class atXLibWindow;
class atWinAPI;

class atWindow
{
  friend atRenderState;
  friend atWin32Window;
  friend atXLibWindow;
  friend atGraphics;
  friend atWinAPI;

public:
  static bool PumpMessage(atWindow *pWindow);
  static int GetResult();

  atWindow(const atWindowCreateInfo &info = atWindowCreateInfo());
  ~atWindow();

  void Clear(const atCol color = 0xFF000000);
  void Clear(const atVec4F &color);
  void Swap();

  void SetTitle(const atString &title);
  void SetPos(const atVec2I &pos);
  void SetSize(const atVec2I &size);
  void SetStyle(const atWindowStyle &style);
  void SetWindowed(const bool &windowed);
  void SetVisible(const bool &visible);
  
  void Maximize();
  void Minimize();
  void Restore();

  double GetDPIScale() const;
  int64_t GetDPI() const;

  atWindowStyle GetStyle() const;

  atString GetTitle() const;

  atVec2I Size() const;
  atVec2I FullSize() const;
  int32_t Width() const;
  int32_t Height() const;

  atVec2I Position() const;
  atVec2I GetScreenPos(const atVec2I &pos) const;
  int32_t GetX() const;
  int32_t GetY() const;

  bool IsMaximized() const;
  bool IsMinimized() const;
  bool IsRestored() const;
  bool IsWindowed() const;
  bool IsVisible() const;

  void SetMenu(atSysMenuHandle hMenu);
  void SetIcon(atSysIconHandle hIcon);
  void SetCursor(atSysCursorHandle hCursor);
  void SetParent(const atWindow &window);
  void SetWndProc(atSysWndCallback callback);

  atSysWndHandle Handle() const;

  bool MakeWindow(const atWindowCreateInfo &info);
  void Destroy();
  void OnResize();

  atCol* Pixels();
  const atCol* Pixels() const;

  atCol& Pixel(const atVec2I &coord);
  atCol& Pixel(const int64_t &index);
  const atCol& Pixel(const atVec2I &coord) const;
  const atCol& Pixel(const int64_t &index) const;

  atGraphics* Graphics() const;

  atVector<atString> DroppedFiles();
  
  bool PumpMessage();

protected:
  void SetHardwareCtx(atGraphics *pGfx);
  void AddDroppedFile(const atString &file);

  atVector<atCol>& PixelsV();
  const atVector<atCol>& PixelsV() const;

  atVector<atString> m_droppedFiles;
  atGraphics *m_pGfx = nullptr;

  atWindowCreateInfo m_createInfo;

#ifdef atPLATFORM_WIN32
  // WINAPI  
  atWin32Window m_sysWindow;
#elif atPLATFORM_LINUX
  atXLibWindow m_sysWindow;
#endif
};

#endif // _atWindow_h__