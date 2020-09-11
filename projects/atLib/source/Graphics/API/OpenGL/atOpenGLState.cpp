#include "atOpenGLState.h"

atOpenGLState::atOpenGLState() {}
atOpenGLState::~atOpenGLState() {}

static void _GLFeature(GLenum feature, bool enable)
{
  if (enable)
    glEnable(feature);
  else
    glDisable(feature);
}

void atOpenGLState::Set(const atRenderState::State &state)
{
  if (!m_initialised || m_curState.viewport != state.viewport) glViewport(state.viewport.x, state.viewport.y, state.viewport.z - state.viewport.x, state.viewport.w - state.viewport.y);

  // Disable scissor test if viewport is entirely contained in the scissor rect
  if (state.viewport.x > state.scissor.x && state.viewport.y > state.scissor.y && state.viewport.z < state.scissor.z && state.viewport.w < state.scissor.w)
  {
    _GLFeature(GL_STENCIL_TEST, false);
  }
  else
  {
    if (!m_initialised || m_curState.scissor != state.scissor)               glScissor(state.scissor.x, state.scissor.y, state.scissor.z - state.scissor.x, state.scissor.w - state.scissor.y);
    if (!m_initialised || m_curState.scissorEnabled != state.scissorEnabled) _GLFeature(GL_SCISSOR_TEST, state.scissorEnabled);
  }

  // GL features
  if (!m_initialised || m_curState.blendEnabled != state.blendEnabled)           _GLFeature(GL_BLEND, state.blendEnabled);
  if (!m_initialised || m_curState.depthReadEnabled != state.depthReadEnabled)   _GLFeature(GL_DEPTH_TEST, state.depthReadEnabled);
  if (!m_initialised || m_curState.depthWriteEnabled != state.depthWriteEnabled) glDepthMask((GLboolean)state.depthWriteEnabled);
  if (!m_initialised || m_curState.msaaEnabled != state.msaaEnabled)             _GLFeature(GL_MULTISAMPLE, state.msaaEnabled);
  if (!m_initialised || m_curState.cullEnabled != state.cullEnabled)             _GLFeature(GL_CULL_FACE, state.cullEnabled);
  if (!m_initialised || m_curState.stencilEnabled != state.stencilEnabled)       _GLFeature(GL_STENCIL_TEST, state.cullEnabled);
  if (!m_initialised || m_curState.depthRange != state.depthRange)               glDepthRange(state.depthRange.x, state.depthRange.y);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  m_initialised = true;
  m_curState = state;
}
