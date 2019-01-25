
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

#ifndef _atRenderTarget_h__
#define _atRenderTarget_h__

#include "atMath.h"
#include "atGraphics.h"
#include "atTextureContext.h"

class atWindow;
class atRenderState;

class atRenderTarget
{
  friend atWindow;
  friend atRenderState;

public:
  atRenderTarget(const int64_t colorTex, const int64_t depthTex = AT_INVALID_ID);
  atRenderTarget(atWindow *pWindow, const atVec2I &size, const bool vsync = true, const bool windowed = true);
  ~atRenderTarget();

  void Clear(const atVec4F &color, const float depth = 1.0f);

  int64_t GetDepthTexID();
  int64_t GetColourTexID();

protected:
  // Use to present the render target to the window (only valid if an atWindow is bound)
  void Swap();
  bool Dirty();

  bool Resize();
  void Destroy();

  // Swap chain is created if the target is an atWindow
  void CreateSwapChain();
  void GetWindowRenderTarget();
  IDXGISwapChain* GetSwapChain();

  int64_t m_colorTexID;
  int64_t m_depthTexID;
  IDXGISwapChain *m_pSwapChain;

  atVec2I m_size;
  bool m_dirty;
  bool m_vsync;
  bool m_windowed;

  atWindow *m_pWindow;
};

#endif