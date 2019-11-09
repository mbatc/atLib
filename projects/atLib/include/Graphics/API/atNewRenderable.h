#ifndef atNewRenderable_h__
#define atNewRenderable_h__

#include "atNewRenderableCore.h"

#ifndef DEFAULT_API
#define DEFAULT_API atGfxApi_DirectX
#endif

class atNewRenderable
{
public:
  atNewRenderable(const atGraphicsAPI api = DEFAULT_API);


protected:
  atVector<atGFXResource*> m_resources;
};

#endif // atNewRenderable_h__
