#ifndef atDXSampler_h__
#define atDXSampler_h__

#include "atGFXSamplerInterface.h"

class atDXSampler : public atGFXSamplerInterface
{
public:
  atGraphicsAPI API() override { return atGfxApi_DirectX; }
};

#endif // atDXSampler_h__
