
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#include "atDirectXState.h"
#include "atOpenGLState.h"
#include "atRenderState.h"
#include "atGraphics.h"
#include "atDirectX.h"
#include "atOpenGL.h"

static atRenderState *s_pGlobalState = nullptr;

atVector<atRenderState::State> atRenderState::m_stack;
atRenderState::State atRenderState::m_activeState;
bool atRenderState::m_setViewport = false;
bool atRenderState::m_immediate = false;

atRenderState::atRenderState(const atGraphicsAPI &api)
{
  if (!s_pGlobalState)
    s_pGlobalState = new atRenderState(0);
  Init();
}

atRenderState::~atRenderState() 
{
  m_stack.pop_back();
  if (m_immediate && m_stack.size())
    Set(m_stack.back());
}

void atRenderState::Bind()
{
  switch (atGraphics::GetCurrent()->GetAPI())
  {
  case atGfxApi_DirectX:
    ((atDirectX*)atGraphics::GetCtx())->GetState()->Set(m_stack.back());
    break;
  case atGfxApi_OpenGL:
    ((atOpenGL*)atGraphics::GetCtx())->GetState()->Set(m_stack.back());
    break;
  }
}

void atRenderState::Init()
{
  m_id = m_stack.size();
  m_stack.push_back(m_stack.size() ? m_stack.back() : State());
  if (m_stack.size() == 1)
    MyState().scissor = { -INT32_MAX, -INT32_MAX, INT32_MAX, INT32_MAX };
}

void atRenderState::SetViewport(const atVec4I &vp) 
{ 
  MyState().viewport = vp;
  if (m_immediate) Bind();
}

void atRenderState::SetScissor(const atVec4I &scissor) 
{ 
  MyState().scissor = scissor; m_setViewport = true;
  if (m_immediate) Bind();
}

void atRenderState::SetDepthRange(const float min, const float max)
{
  MyState().depthRange = atVec2F(min, max);
  if (m_immediate) Bind();
}

void atRenderState::SetDepthWriteEnabled(const bool enabled) 
{ 
  MyState().depthWriteEnabled = enabled;
  if (m_immediate) Bind();
}

void atRenderState::SetDepthReadEnabled(const bool enabled)
{
  MyState().depthReadEnabled = enabled;
  if (m_immediate) Bind();
}

void atRenderState::SetStencilEnabled(const bool enabled)
{
  MyState().stencilEnabled = enabled;
  if (m_immediate) Bind();
}

void atRenderState::SetScissorEnabled(const bool enabled)
{
  MyState().scissorEnabled = enabled;
  if (m_immediate) Bind();
}

void atRenderState::SetBlendEnabled(const bool enabled)
{
  MyState().blendEnabled = enabled;
  if (m_immediate) Bind();
}

void atRenderState::SetMSAAEnabled(const bool enabled)
{
  MyState().msaaEnabled = enabled;
  if (m_immediate) Bind();
}

void atRenderState::SetCullEnabled(const bool enabled)
{
  MyState().cullEnabled = enabled;
  if (m_immediate) Bind();
}

void atRenderState::SetAAEnabled(const bool enabled)
{
  MyState().aaEnabled = enabled;
  if (m_immediate) Bind();
}

void atRenderState::Set(const State &state)
{
  m_stack[m_id] = state;
  if (m_immediate) Bind();
}

bool atRenderState::IsDepthWriteEnabled() const { return MyState().depthWriteEnabled; }
bool atRenderState::IsDepthReadEnabled() const { return MyState().depthReadEnabled; }
bool atRenderState::IsStencilEnabled() const { return MyState().stencilEnabled; }
bool atRenderState::IsScissorEnabled() const { return MyState().scissorEnabled; }
bool atRenderState::IsBlendEnabled() const { return MyState().blendEnabled; }
bool atRenderState::IsMSAAEnabled() const { return MyState().msaaEnabled; }
bool atRenderState::IsCullEnabled() const { return MyState().cullEnabled; }
bool atRenderState::IsAAEnabled() const { return MyState().aaEnabled; }
const atVec4I& atRenderState::Viewport() const { return MyState().viewport; }
const atVec4I& atRenderState::Scissor() const { return MyState().scissor; }
const atVec2F& atRenderState::DepthRange() const { return MyState().depthRange; }
const atRenderState::State& atRenderState::MyState() const { return m_stack[m_id]; }
atRenderState::State& atRenderState::MyState() { return m_stack[m_id]; }
atRenderState::atRenderState(int unused) { Init(); }

struct CleanupStruct
{
  ~CleanupStruct()
  {
    delete s_pGlobalState;
    s_pGlobalState = nullptr;
  }
};

static CleanupStruct cleanup;