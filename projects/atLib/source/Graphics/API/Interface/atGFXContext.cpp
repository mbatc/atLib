#include "atGFXContext.h"
#include "atDirectX.h"
#include "atOpenGL.h"

atGFXContext* atGFXContext::Create(const atGraphicsAPI& apiID, atWindow *pWindow, const bool &vsync)
{
  switch (apiID)
  {
  case atGfxApi_DirectX: return atNew<atDirectX>(pWindow, vsync);
  case atGfxApi_OpenGL: return atNew<atOpenGL>(pWindow, vsync);
  }
  return nullptr;
}
