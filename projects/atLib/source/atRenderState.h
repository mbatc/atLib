
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

#include "atGraphics.h"
#include "atMath.h"
#include "atShader.h"

class atRenderState
{
public:
  atRenderState() = delete;

  static void Bind();
  static void BindShader(const int64_t id);

  static void SetViewport(const atVec4I &viewport, const bool updateFlag = true);
  static void SetDepthRange(const float min, const float max);

  static void EnableDepthTest(const bool enable);
  static void EnableCulling(const bool enable);
  static void EnableBlend(const bool enable);
  static void EnableScissor(const bool enable);
  static void EnableAA(const bool enable);
  static void EnableMultisample(const bool enable);

  static bool ViewportSet();
  static bool ScissorSet();

protected:
  static void SetDefaults();

  static bool m_viewDirty;
  static bool m_defaultSet;
  static bool m_scissorSet;
  static bool m_viewportSet;

  static ID3D11DepthStencilState *m_pDepthState;
  static ID3D11RasterizerState *m_pRasterState;
  static ID3D11BlendState *m_pBlendState;

  static D3D11_DEPTH_STENCIL_DESC m_depthDesc;
  static D3D11_RASTERIZER_DESC m_rasterDesc;
  static D3D11_BLEND_DESC m_blendDesc;

  static D3D11_DEPTH_STENCIL_DESC m_lastDepthDesc;
  static D3D11_RASTERIZER_DESC m_lastRasterDesc;
  static D3D11_BLEND_DESC m_lastBlendDesc;
  static D3D11_VIEWPORT m_viewport;

  static D3D11_CULL_MODE m_lastCullMode;
  static int64_t m_shader;
};

#endif