#ifndef atDirectXState_h__
#define atDirectXState_h__

#ifdef atPLATFORM_WIN32

#include "atDirectX.h"
#include "atGFXContextState.h"

class atDirectXState : public atGFXContextState
{
public:
  atDirectXState();
  ~atDirectXState();

  void Set(const atRenderState::State &state) override;

protected:
  void *m_pImpl = nullptr;
};

#endif

#endif // atDirectXState_h__
