#ifndef atGLShader_h__
#define atGLShader_h__

#include "atGFXShaderInterface.h"

class atGLShader : public atGFXShaderInterface
{
public:
  atGLShader(const atString &src = "", const atPipelineStage &stage = atPS_None);

  bool Bind() override;
  bool Upload() override;
  bool Delete() override;

  int64_t Target();

  atGraphicsAPI API() override { return atGfxApi_OpenGL; }
};

#endif // atGLShader_h__
