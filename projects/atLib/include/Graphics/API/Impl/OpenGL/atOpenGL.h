#ifndef atOpenGL_h__
#define atOpenGL_h__

#include "../../3rdParty/glew/include/GL/glew.h"
#if defined _WIN32 || defined _WIN64
#include "../../3rdParty/glew/include/GL/wglew.h"
#endif

#include "atMath.h"
#include "atGFXPrgmInterface.h"

#define VERIFY_GL() { uint32_t err = glGetError(); atRelAssert(err == GL_NONE, ); }

class atWindow;
class atOpenGLState;

class atOpenGL
{
public:
  atOpenGL(atWindow *pWindow, const bool &vsyncEnabled = true);
  ~atOpenGL();

  void Swap();

  void DrawIndexed(int64_t nIndices, int64_t startLocation = 0, int64_t baseVertIndex = 0, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList, const atType &indicesType = atType_Uint32);
  void Draw(int64_t nVerts, int64_t startLocation = 00, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList);
  
  atOpenGLState* GetState();

protected:
  HGLRC m_hGL = 0;
  HWND m_hWnd = 0;
  HDC m_hDC = 0;

  atOpenGLState *m_pState;
};

#endif // atOpenGL_h__
