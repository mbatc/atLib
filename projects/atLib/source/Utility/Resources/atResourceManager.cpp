#include "atResourceManager.h"
#include "atMesh.h"
#include "atImage.h"
#include "atTexture.h"
#include "atFont.h"

atResourceManager::atResourceManager(const bool &addDefaultHandlers)
{
  if (addDefaultHandlers)
  {
    AddHandler<atResourceHandlers::MeshHandler>();
    AddHandler<atResourceHandlers::ImageHandler>();
    AddHandler<atResourceHandlers::TextureHandler>();
    AddHandler<atResourceHandlers::FontHandler>();
  }
}
