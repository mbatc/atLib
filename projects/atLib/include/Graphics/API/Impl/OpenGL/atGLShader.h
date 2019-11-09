#ifndef atGLShader_h__
#define atGLShader_h__

#include "atGFXShaderInterface.h"

class atGLShader : public atGFXShaderInterface
{
public:
  atGraphicsAPI API() override { return atGfxApi_OpenGL; }
};

#endif // atGLShader_h__
