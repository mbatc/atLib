#ifdef atPLATFORM_LINUX

#include "atOpenGLState.h"
#include "atOpenGL.h"
#include "atWindow.h"
#include "atInput.h"
#include "atFormat.h"
#include "atRenderState.h"
#include "X11/xlib.h"

// Linux OpenGL context implementation
struct atGLImpl
{
  int hWnd = 0;
  XVisualInfo = 0;
  GLXContext hDC = 0;
};

atGLCtx atOpenGL::CreateContext(atWindow *pWindow, const bool &vsyncEnabled)
{
}

void atOpenGL::DestroyContext(atGLCtx ctx)
{
}

void atOpenGL::Swap(atGLCtx ctx)
{
}

void atOpenGL::BindContext(atGLCtx ctx)
{
}

#endif
