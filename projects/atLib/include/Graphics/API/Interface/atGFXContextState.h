#ifndef atGFXContextState_h__
#define atGFXContextState_h__

#include "atRenderState.h"

class atGFXContextState
{
public:
  virtual void Set(const atRenderState::State &state) = 0;
};

#endif // atGFXContextState_h__
