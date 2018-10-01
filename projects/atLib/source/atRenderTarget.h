
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

#include "atGraphics.h"
#include "atMath.h"

class atWindow;

class atRenderTarget
{
public:
  atRenderTarget(const atWindow *pWindow, const atVec2I &size, const bool vsync = true, const bool windowed = true);
  ~atRenderTarget();

  void Bind();
  void Swap();
  void Clear(const atVec4F &color);

protected:
  bool Resize();
  void Destroy();

  ID3D11DepthStencilView* GetDepthStencilView();
  ID3D11RenderTargetView* GetRenderTarget();
  ID3D11Texture2D* GetDepthStencilBuffer();
  IDXGISwapChain* GetSwapChain();

  void CreateSwapChain();
  void CreateDepthStencil();
  void CreateRenderTarget();

  ID3D11DepthStencilView *m_pDepthStencilView;
  ID3D11RenderTargetView *m_pRenderTarget;
  ID3D11Texture2D *m_pDepthStencilBuffer;
  IDXGISwapChain *m_pSwapChain;

  atVec2I m_size;
  bool m_vsync;
  bool m_windowed;

  const atWindow *m_pWindow;
};

#endif