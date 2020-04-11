#include "atOpenGLState.h"
#include "atOpenGL.h"
#include "atWindow.h"
#include "atInput.h"
#include "atFormat.h"
#include "atRenderState.h"

static void GLAPIENTRY _ErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

atOpenGL::~atOpenGL()
{
  atDelete(m_pState);
  DestroyContext(m_ctx);
}

atOpenGL::atOpenGL(atWindow *pWindow, const bool &vsyncEnabled)
{
  m_ctx = CreateContext(pWindow, vsyncEnabled);
  m_pState = atNew<atOpenGLState>();

#ifdef _DEBUG // Enable error logging in debug
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(_ErrorMessageCallback, 0);
#endif
}

static int64_t _GetPolygonMode(const atGFX_PrimitiveType &primType)
{
  switch (primType)
  {
  case atGFX_PT_LineList: return GL_LINE;
  case atGFX_PT_LineListAdj: return GL_LINES_ADJACENCY;
  case atGFX_PT_PointList: return GL_POINT;
  case atGFX_PT_TriangleList: return GL_TRIANGLES;
  case atGFX_PT_TriangleListAdj: return GL_TRIANGLES_ADJACENCY;
  case atGFX_PT_TriangleStrip: return GL_TRIANGLE_STRIP;
  }

  return GL_TRIANGLES;
}

void atOpenGL::Swap() { Swap(m_ctx); }

void atOpenGL::DrawIndexed(int64_t nIndices, int64_t startLocation, int64_t baseVertIndex, const atGFX_PrimitiveType &primType, const atType &indicesType)
{
  atRenderState::Bind();
  glDrawElements((GLenum)_GetPolygonMode(primType), (GLsizei)nIndices, (GLenum)atFormat::GLType(indicesType), 0);
}

void atOpenGL::Draw(int64_t nVerts, int64_t startLocation, const atGFX_PrimitiveType &primType)
{
  atRenderState::Bind();
  glDrawArrays((GLenum)_GetPolygonMode(primType), (GLint)startLocation, (GLsizei)nVerts);
}

atGFXContextState* atOpenGL::GetState() { return m_pState; }

void atOpenGL::ClearColour(const atVec4F &colour)
{
  glClearColor(colour.x, colour.y, colour.z, colour.w);
  glClear(GL_COLOR_BUFFER_BIT);
}

void atOpenGL::ClearDepth(const float &depth)
{
  glClearDepth(depth);
  glClear(GL_DEPTH_BUFFER_BIT);
}

void atOpenGL::ClearStencil()
{
  glClear(GL_STENCIL_BUFFER_BIT);
}

void atOpenGL::Resize(const atVec2I &) { /* Nothing to do. OpenGL resizes the swap chain automatically */ }

void atOpenGL::SetWindowed(const bool &windowed)
{
  atUnused(windowed);
  // Not sure what to do here yet.
}

atGraphicsAPI atOpenGL::API() { return atGfxApi_OpenGL; }

static void GLAPIENTRY _ErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
  atUnused(userParam, length);
  atString assertion = "OpenGL Error (type: " + atString((int64_t)type) + ", severity: " + atString((int64_t)severity) + ", id: " + atString((int64_t)id) + ")\n\n" + message;
  printf((assertion + "\n").c_str());
  atRelAssert(type != GL_DEBUG_TYPE_ERROR, assertion);
}
