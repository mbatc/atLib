#ifndef atOpenGLState_h__
#define atOpenGLState_h__

#include "atOpenGL.h"
#include "atRenderState.h"

class atOpenGLState
{
public:
  atOpenGLState();
  ~atOpenGLState();

  void Set(const atRenderState::State &state);

protected:
  atRenderState::State m_curState;
  bool m_initialised = false;
};

#endif // atOpenGLState_h__
