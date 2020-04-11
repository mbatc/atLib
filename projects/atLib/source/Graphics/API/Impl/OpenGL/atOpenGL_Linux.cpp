#include "atOpenGLState.h"
#include "atOpenGL.h"
#include "atWindow.h"
#include "atInput.h"
#include "atFormat.h"
#include "atRenderState.h"

#ifdef atPLATFORM_LINUX
#include "X11/xlib.h"

// Linux OpenGL context implementation
struct atGLImpl
{
  int hWnd = 0;
  XVisualInfo = 0;
  GLXContext hDC = 0;
};

static void GLAPIENTRY _ErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
  atUnused(userParam, length);
  atString assertion = "OpenGL Error (type: " + atString((int64_t)type) + ", severity: " + atString((int64_t)severity) + ", id: " + atString((int64_t)id) + ")\n\n" + message;
  printf((assertion + "\n").c_str());
  atRelAssert(type != GL_DEBUG_TYPE_ERROR, assertion);
}

static HGLRC _CreateContext(HWND hWnd, HDC *pDC)
{
  HGLRC glCtx = 0;
  {
    HGLRC tempGLCtx;
    atWindow tempWnd("FakeWnd", { 800, 600 }, { 0 }, true, false);
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

    *pDC = hDC;

    wglMakeCurrent(hDC, glCtx);
    glewInit();
  }

  while (atInput::Update()); // wait for tempWnd messages to be flushed

  return glCtx;
}

atOpenGL::atOpenGL(atWindow *pWindow, const bool &vsyncEnabled)
{
  m_hWnd = pWindow->Handle();
  m_hGL = _CreateContext(m_hWnd, &m_hDC);
  m_pState = atNew<atOpenGLState>();

#ifdef _DEBUG // Enable error logging in debug
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(_ErrorMessageCallback, 0);
#endif
}

atOpenGL::~atOpenGL()
{
  atDelete(m_pState);
  if (wglGetCurrentContext() == m_hGL) // unbind the hDC or glContext if either is current
    wglMakeCurrent(wglGetCurrentDC(), nullptr);
  else if (wglGetCurrentDC() == m_hDC)
    wglMakeCurrent(nullptr, wglGetCurrentContext());

  // Delete the context and free the DC handle
  wglDeleteContext(m_hGL);
  ReleaseDC(m_hWnd, m_hDC);
}

#endif
