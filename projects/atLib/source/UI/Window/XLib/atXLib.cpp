#include "atXLib.h"

#ifdef atPLATFORM_LINUX

static Display *_pDisplay = nullptr;
static int64_t _initCount = 0;

atXLibWindow::atXLibWindow(atWindow *pWindow) : m_pWindow(pWindow)
{
  if (++_initCount == 1)
    _pDisplay = XOpenDisplay(0); // Open default display
}

atXLibWindow::~atXLibWindow()
{
  if (--_initCount == 0)
    XCloseDisplay(_pDisplay);
}

void atXLibWindow::Clear(const atCol &) {}
void atXLibWindow::Swap() {}

bool atXLibWindow::Create(const atWindowCreateInfo &info)
{
  Window root = DefaultRootWindow(_pDisplay);

  XVisualInfo *pVI = XMatchVisualInfo(_pDisplay, 0, info.depthBits, InputOutput);
  if (pVI == 0)
    return false;

  ColorMap colMap = XCreateColormap(_pDisplay, root, pVI->visual, AllocNone);

  XSetWindowAttributes attributes;
  attributes.background_pixel = 0;
  attributes.border_pixel = 0;
  attributes.colormap = colMap;
  attributes.event_mask = ExposureMask | KeyPressMask;

  m_hWnd = XCreateWindow(
    _pDisplay,
    root,
    (int)info.pos.x, (int)info.pos.y,
    (uint32_t)info.size.x, (uint32_t)info.size.y,
    1,
    info.depthBits, 
    InputOutput,
    pVI->visual,
    CWBackPixel | CWBorderPixel | CWEventMask | CW_Colormap,
    &attributes);

  if (m_hWnd == 0)
    return false;
  
  XMapWindow(_pDisplay, m_hWnd);
  XStoreName(_pDisplay, m_hWnd, info.title.c_str());
}

void atXLibWindow::Destroy()
{
  if (m_hWnd != 0)
    XDestroyWindow(_pDisplay, m_hWnd);
  m_hWnd = 0;
}

void atXLibWindow::SetTitle(const atString &title) {}
void atXLibWindow::OnResize() {}
  
void atXLibWindow::SetWindowRect(const atVec4I &rect) {}
void atXLibWindow::SetWindowed(const bool &windowed) {}
void atXLibWindow::SetVisible(const bool &visible) {}
void atXLibWindow::SetStyle(const atWindowStyle &style) {}

void atXLibWindow::Maximize() {}
void atXLibWindow::Minimize() {}
void atXLibWindow::Restore() {}

void atXLibWindow::SetParent(const atSysWndHandle &hParent) {}
void atXLibWindow::SetCursor(const atSysCursorHandle &hCursor) {}
void atXLibWindow::SetMenu(const atSysMenuHandle &hMenu) {}
void atXLibWindow::SetIcon(const atSysIconHandle &jIcon) {}
void atXLibWindow::SetCallback(const atSysWndCallback &callback) {}

atWindowStyle atXLibWindow::GetStyle() const { return atWindowStyle(0); }

atString atXLibWindow::GetTitle() const { return ""; }

atVec2I atXLibWindow::GetSize() const { return 0; }
atVec2I atXLibWindow::GetPos() const { return 0; }

bool atXLibWindow::IsMaximized() const { return false; }
bool atXLibWindow::IsMinimized() const { return false; }
bool atXLibWindow::IsWindowed() const { return false; }
bool atXLibWindow::IsVisible() const { return false; }

atSysWndHandle atXLibWindow::Handle() const { return m_hWnd; }
atVector<atCol>& atXLibWindow::Pixels() { return m_pixels; }
const atVector<atCol>& atXLibWindow::Pixels() const { return m_pixels; }

#endif