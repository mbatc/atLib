#ifndef atDXPrgm_h__
#define atDXPrgm_h__

#include "atGFXPrgmInterface.h"

class atGLPrgm : public atGFXPrgmInterface
{
public:
  atGraphicsAPI API() override { return atGfxApi_OpenGL; }

protected:

};

#endif // atDXPrgm_h__
