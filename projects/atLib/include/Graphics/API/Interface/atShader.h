#ifndef atShader_h__
#define atShader_h__

#include "atString.h"
#include "atGFXResource.h"
#include "atTexture.h"
#include "atSampler.h"

class atShader : public atGFXResource
{
public:
  atShader(const atPipelineStage &stage = atPS_None) : m_src(""), m_isFile(false), m_stage(stage) {}
  void SetSource(const atString &src);
  void SetSourceFile(const atFilename &file);

  const atPipelineStage& Stage() { return m_stage; }

protected:
  atString LoadSource() const;

  atPipelineStage m_stage = atPS_None;
  atString m_src;
  bool m_isFile = false;
};

#endif // atShader_h__
