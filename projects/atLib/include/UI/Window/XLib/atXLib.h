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

  bool Create();
  void Destroy();
  void SetTitle(const atString &title);
  void OnResize();
  
  void SetWindowRect(const atVec4I &rect);
  void SetWindowed(const bool &windowed);
  void SetVisible(const bool &visible);

  void Maximize();
  void Minimize();
  void Restore();

  void SetParent(atSysWndHandle hParent);
  void SetCursor(atSysCursorHandle hParent);
  void SetMenu(atSysMenuHandle hParent);
  void SetIcon(atSysIconHandle hParent);
  void SetCallback(atSysWndCallback callback);

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
};

#endif