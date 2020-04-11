#ifndef atOpenGL_h__
#define atOpenGL_h__

#include "../../3rdParty/glew/include/GL/glew.h"
#if defined _WIN32 || defined _WIN64
#include "../../3rdParty/glew/include/GL/wglew.h"
#endif

#include "atMath.h"
#include "atGFXContext.h"

class atWindow;
class atOpenGLState;

struct atGLImpl;
typedef atGLImpl* atGLCtx;

class atOpenGL : public atGFXContext
{
public:
  // Create a new OpenGL context bound to the given window
  atOpenGL(atWindow *pWindow, const bool &vsyncEnabled = true);
  ~atOpenGL();

  // Swap the windows buffers, presenting the frame
  void Swap();

  // Execute a draw command using the current OpenGL state
  void DrawIndexed(int64_t nIndices, int64_t startLocation = 0, int64_t baseVertIndex = 0, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList, const atType &indicesType = atType_Uint32);
  void Draw(int64_t nVerts, int64_t startLocation = 00, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList);
  
  // Return the state of this OpenGL context
  atGFXContextState* GetState();

  void ClearColour(const atVec4F &colour) override;
  void ClearDepth(const float &colour) override;
  void ClearStencil() override;
  void Resize(const atVec2I &size) override;
  void SetWindowed(const bool &windowed) override;

  atGraphicsAPI API() override;

protected:
  atOpenGLState *m_pState;

  // These functions implement functionality that needs to be OS specific
  static atGLCtx CreateContext(atWindow *pWindow, const bool &vsyncEnabled = true);
  static void DestroyContext(atGLCtx ctx);
  static void Swap(atGLCtx ctx);

  // atGLCtx implementation is hidden from the user as the definition
  // of this struct will change depending on the platform
  //
  // Platform specific aspects of OpenGL are hidden from the user this way
  atGLCtx m_ctx;
};

#endif // atOpenGL_h__
