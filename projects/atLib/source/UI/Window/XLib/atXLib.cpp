#include "atXLib.h"

#ifdef atPLATFORM_LINUX

atXLibWindow::atXLibWindow(atWindow *pWindow) : m_pWindow(pWindow) {}
~atXLibWindow::atXLibWindow() {}

void atXLibWindow::Clear(const atCol &) {}
void atXLibWindow::Swap() {}

bool atXLibWindow::Create() { return false; }
void atXLibWindow::Destroy() {}
void atXLibWindow::SetTitle(const atString &) {}
void atXLibWindow::OnResize() {}
  
void atXLibWindow::SetWindowRect(const atVec4I &) {}
void atXLibWindow::SetWindowed(const bool &) {}
void atXLibWindow::SetVisible(const bool &) {}

void atXLibWindow::Maximize() {}
void atXLibWindow::Minimize() {}
void atXLibWindow::Restore() {}

void atXLibWindow::SetParent(atSysWndHandle hParent) {}
void atXLibWindow::SetCursor(atSysCursorHandle hParent) {}
void atXLibWindow::SetMenu(atSysMenuHandle hParent) {}
void atXLibWindow::SetIcon(atSysIconHandle hParent) {}
void atXLibWindow::SetCallback(atSysWndCallback callback) {}

atWindowStyle atXLibWindow::GetStyle() const { return atWindowStyle(0); }

atString atXLibWindow::GetTitle() const { return ""; }

atVec2I atXLibWindow::GetSize() const { return 0; }
atVec2I atXLibWindow::GetPos() const { return 0; }

bool atXLibWindow::IsMaximized() const { return false; }
bool atXLibWindow::IsMinimized() const { return false; }
bool atXLibWindow::IsWindowed() const { return false; }
bool atXLibWindow::IsVisible() const { return false; }

atSysWndHandle atXLibWindow::Handle() const { return 0; }
const atVector<atCol>& atXLibWindow::Pixels() { return m_pixels; }

#endif