#ifndef atDirectXState_h__
#define atDirectXState_h__

#ifdef atPLATFORM_WIN32

#include "atDirectX.h"
#include "atRenderState.h"

struct __atDXStateImpl;

class atDirectXState
{
public:
  atDirectXState();
  ~atDirectXState();

  void Set(const atRenderState::State &state);

protected:
  __atDXStateImpl *m_pDX = nullptr;
};

#endif

#endif // atDirectXState_h__
