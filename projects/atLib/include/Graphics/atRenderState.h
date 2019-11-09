
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
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

#ifndef _atRenderState_h__
#define _atRenderState_h__


#include "atWindow.h"
#include "atShader.h"
#include "atMath.h"

class atRenderState
{
  friend atGraphics;
  friend atDirectX;
  atRenderState(int unused);
public:
  struct State
  {
    atVec4I viewport = { -1, -1, -1, -1 };
    atVec4I scissor = { 0, 0, 0, 0 };
    atVec2F depthRange = { 0.0f, 1.0f };

    int64_t shader = AT_INVALID_ID;
    int64_t inputLayout = AT_INVALID_ID;

    bool depthWriteEnabled = true;
    bool depthReadEnabled = true;
    bool stencilEnabled = false;
    bool scissorEnabled = false;
    bool cullEnabled = false;
    bool blendEnabled = false;
    bool msaaEnabled = true;
    bool aaEnabled = true;
    
    atRenderTarget *pColourTarget;
    atRenderTarget *pDepthTarget;
  };

  atRenderState();
  ~atRenderState();

  void Set(const State &state);

  void SetRenderTarget(atWindow *pTarget);
  void SetRenderTarget(atRenderTarget *pTarget);

  bool SetShader(const int64_t id, const int64_t inputLayoutID);
  void SetViewport(const atVec4I &vp);
  void SetScissor(const atVec4I &scissor);
  void SetDepthRange(const float min, const float max);

  void SetDepthWriteEnabled(const bool enabled);
  void SetDepthReadEnabled(const bool enabled);
  void SetStencilEnabled(const bool enabled);
  void SetScissorEnabled(const bool enabled);
  void SetBlendEnabled(const bool enabled);
  void SetMSAAEnabled(const bool enabled);
  void SetCullEnabled(const bool enabled);
  void SetAAEnabled(const bool enabled);

  bool IsDepthWriteEnabled() const;
  bool IsDepthReadEnabled() const;
  bool IsStencilEnabled() const;
  bool IsScissorEnabled() const;
  bool IsBlendEnabled() const;
  bool IsMSAAEnabled() const;
  bool IsCullEnabled() const;
  bool IsAAEnabled() const;

  const atVec4I &Viewport() const;
  const atVec4I &Scissor() const;
  const atVec2F &DepthRange() const;

  static bool m_alwaysBind;

protected:
  void Init();
  State &MyState();
  const State &MyState() const;

private:
  static void Bind();

  static int64_t m_shader;
  static atVector<State> m_stack;
  static State m_activeState;
  static bool m_setViewport;

  int64_t m_id;
};

#endif