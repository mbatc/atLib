#ifndef atShader_h__
#define atShader_h__

#include "atString.h"
#include "atGFXResource.h"
#include "atTexture.h"
#include "atSampler.h"

class atShader : public atGFXResource
{
public:
  atShader(const atString &src = "", const atPipelineStage &stage = atPS_None) : m_src(src), m_stage(stage) {}
  
  const atPipelineStage& Stage() { return m_stage; }

protected:
  atPipelineStage m_stage = atPS_None;
  atString m_src;
};

#endif // atShader_h__
