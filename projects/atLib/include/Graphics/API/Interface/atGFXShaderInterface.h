#ifndef atGFXShaderInterface_h__
#define atGFXShaderInterface_h__

#include "atString.h"
#include "atGFXResource.h"
#include "atGFXTexInterface.h"
#include "atGFXSamplerInterface.h"

enum atPipelineStage
{
  atPS_Vertex,
  atPS_Fragment,
  atPS_Geometry,
  atPS_Compute,
  atPS_Domain,
  atPS_Hull,
  atPS_Count,
  atPS_None,
};

class atGFXShaderInterface : public atGFXResource
{
public:
  atGFXShaderInterface(const atString &src = "", const atPipelineStage &stage = atPS_None) : m_src(src), m_stage(stage) {}
  
  const atPipelineStage& Stage() { return m_stage; }

protected:
  atPipelineStage m_stage = atPS_None;
  atString m_src;
};

#endif // atGFXShaderInterface_h__
