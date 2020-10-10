#include "atOpenGLState.h"
#include "atOpenGL.h"
#include "atWindow.h"
#include "atInput.h"
#include "atFormat.h"
#include "atRenderState.h"

#include "atGLBuffer.h"
#include "atGLPrgm.h"
#include "atGLTexture.h"
#include "atGLShader.h"
#include "atGLRenderTarget.h"

static void GLAPIENTRY _ErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

atOpenGL::~atOpenGL()
{
  atDelete(m_pState);
  DestroyContext(m_ctx);
}

atOpenGL::atOpenGL(atWindow *pWindow, const bool &vsyncEnabled)
  : atGraphics(pWindow, vsyncEnabled)
{
  m_ctx = CreateContext(pWindow, vsyncEnabled);
  m_pState = atNew(atOpenGLState);

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
  glDrawElementsBaseVertex((GLenum)_GetPolygonMode(primType), (GLsizei)nIndices, (GLenum)atFormat::GLType(indicesType), (void*)(startLocation * atSize(indicesType)),(GLint)baseVertIndex);
}

void atOpenGL::Draw(int64_t nVerts, int64_t startLocation, const atGFX_PrimitiveType &primType)
{
  atRenderState::Bind();
  glDrawArrays((GLenum)_GetPolygonMode(primType), (GLint)startLocation, (GLsizei)nVerts);
}

atGFXContextState* atOpenGL::GetState() { return m_pState; }

void atOpenGL::ClearColour(const atVec4F &colour)
{
  glClearColor(colour.x, colour.y, colour.z, 0.0f);
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

void atOpenGL::BindContext() { BindContext(m_ctx); }

atGraphicsAPI atOpenGL::API() { return atGfxApi_OpenGL; }

atSampler*      atOpenGL::CreateSampler()                                             { return nullptr; }
atGPUBuffer*    atOpenGL::CreateBuffer(const atBufferType &type, const int64_t &size) { return atNew(atGLBuffer)(type, size); }
atTexture*      atOpenGL::CreateTexture(const atTextureType &type)                    { return atNew(atGLTexture)(type); }
atShader*       atOpenGL::CreateShader(const atPipelineStage &stage)                  { return atNew(atGLShader)(stage); }
atProgram*      atOpenGL::CreateProgram()                                             { return atNew(atGLPrgm); }
atRenderTarget* atOpenGL::CreateRenderTarget()                                        { return atNew(atGLRenderTarget); }

static void GLAPIENTRY _ErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
  atUnused(userParam, length);
  atString assertion = "OpenGL Error (type: " + atString((int64_t)type) + ", severity: " + atString((int64_t)severity) + ", id: " + atString((int64_t)id) + ")\n\n" + message;
  printf((assertion + "\n").c_str());
  atRelAssert(type != GL_DEBUG_TYPE_ERROR, assertion);
}
