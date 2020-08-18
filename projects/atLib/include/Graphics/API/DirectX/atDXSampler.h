#ifndef atDXSampler_h__
#define atDXSampler_h__

#ifdef atPLATFORM_WIN32

#include "atSampler.h"

class atDXSampler : public atSampler
{
public:
  atGraphicsAPI API() override { return atGfxApi_DirectX; }

  bool Upload() override;
  bool Delete() override;
};

#endif

#endif // atDXSampler_h__
