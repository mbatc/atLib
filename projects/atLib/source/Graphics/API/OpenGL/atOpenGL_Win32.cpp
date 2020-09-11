#ifdef atPLATFORM_WIN32
#include "atOpenGLState.h"
#include "atOpenGL.h"
#include "atWindow.h"
#include "atInput.h"
#include "atFormat.h"
#include "atRenderState.h"

#include "../../3rdParty/glew/include/GL/wglew.h"

// Simple struct that holds OS specific gl data
struct atGLImpl
{
  HGLRC hGL = 0;
  HWND hWnd = 0;
  HDC hDC = 0;
};

atGLCtx atOpenGL::CreateContext(atWindow *pWindow, const bool &vsyncEnabled)
{
  atGLImpl *pCtx = atNew<atGLImpl>();

  HWND hWnd = pWindow->Handle();
  HGLRC glCtx = 0;

  {
    HGLRC tempGLCtx;
    atWindowCreateInfo info;
    info.title = "FakeWnd";
    info.size = { 800, 600 };
    info.pos = { 0 };
    info.flags = atWF_None;
    atWindow tempWnd(info);
    HDC hTempDC = GetDC(tempWnd.Handle());

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 24;
    int pfdID = ChoosePixelFormat(hTempDC, &pfd);

    SetPixelFormat(hTempDC, pfdID, &pfd);

    tempGLCtx = wglCreateContext(hTempDC);
    wglMakeCurrent(hTempDC, tempGLCtx);

    PFNWGLCHOOSEPIXELFORMATARBPROC glChoosePixelFormatARB = nullptr;
    glChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));

    PFNWGLCREATECONTEXTATTRIBSARBPROC glCreateContextAttribsARB = nullptr;
    glCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));

    const int pixelAttribs[] = {
      WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
      WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
      WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
      WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
      WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
      WGL_COLOR_BITS_ARB, 32,
      WGL_ALPHA_BITS_ARB, 8,
      WGL_DEPTH_BITS_ARB, 24,
      WGL_STENCIL_BITS_ARB, 8,
      WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
      WGL_SAMPLES_ARB, 4,
      0
    };

    int pixelFormatID; UINT numFormats;

    HDC hDC = GetDC(hWnd);
    glChoosePixelFormatARB(hDC, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);

    memset(&pfd, 0, sizeof(pfd));
    DescribePixelFormat(hTempDC, pixelFormatID, sizeof(pfd), &pfd);
    SetPixelFormat(hDC, pixelFormatID, &pfd);

    const int major_min = 4, minor_min = 5;
    int contextAttribs[] = {
      WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
      WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
      WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
      0
    };

    glCtx = glCreateContextAttribsARB(hDC, nullptr, contextAttribs);

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(tempGLCtx);
    ReleaseDC(tempWnd.Handle(), hTempDC);

    wglMakeCurrent(hDC, glCtx);
    glewInit();

    pCtx->hDC = hDC;
    pCtx->hWnd = hWnd;
    pCtx->hGL = glCtx;
  }

  while (atInput::Update()); // wait for tempWnd messages to be flushed

  return pCtx;
}

void atOpenGL::DestroyContext(atGLCtx ctx)
{
  if (wglGetCurrentContext() == ctx->hGL) // unbind the hDC or glContext if either is current
    wglMakeCurrent(wglGetCurrentDC(), nullptr);
  else if (wglGetCurrentDC() == ctx->hDC)
    wglMakeCurrent(nullptr, wglGetCurrentContext());

  // Delete the context and free the DC handle
  wglDeleteContext(ctx->hGL);
  ReleaseDC(ctx->hWnd, ctx->hDC);
  atDelete(ctx);
}

void atOpenGL::Swap(atGLCtx ctx) { SwapBuffers(ctx->hDC); }

void atOpenGL::BindContext(atGLCtx ctx) { wglMakeCurrent(ctx->hDC, ctx->hGL); }

#endif
