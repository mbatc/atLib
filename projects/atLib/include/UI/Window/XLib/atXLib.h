#ifndef _atXLib_h__
#define _atXLib_h__

#include "atPlatform.h"
#include "atWindowDefinitions.h"
#include "atColor.h"
#include "atString.h"

class atWindow;

class atXLibWindow
{
public:
  atXLibWindow(atWindow *pWindow);
  ~atXLibWindow();

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

  atVec2I GetSize() const;
  atVec2I GetPos() const;

  bool IsMaximized() const;
  bool IsMinimized() const;
  bool IsWindowed() const;
  bool IsVisible() const;

  atSysWndHandle Handle() const;
  const atVector<atCol>& Pixels();
  
protected:
  // Hi-level window
  atVector<atCol> m_pixels;
  atVector<atString> m_droppedFiles;
  atWindow *m_pWindow;

  atSysWndHandle m_hWnd = 0;
};

#endif