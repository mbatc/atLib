
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

#include "atGraphics.h"
#include "atDirectX.h"
#include "atOpenGL.h"
#include "atWindow.h"

static atGraphics *_pCurrent = nullptr;

class __atGfxImpl
{
public:
  __atGfxImpl(atWindow *pWindow, const atGraphicsAPI &api);
  ~__atGfxImpl();

  atGraphicsAPI m_api;

  union
  {
    atOpenGL *m_pOpenGL;
    atDirectX *m_pDirectX;
  };
};

atGraphics::atGraphics(atWindow *pWindow, const atGraphicsAPI &api)
  : m_pWindow(pWindow)
  , m_api(api)
  , m_pImpl(atNew<__atGfxImpl>(pWindow, api))
{
  if (!_pCurrent)
    SetCurrent(this);
  pWindow->SetHardwareCtx(this);
}

atGraphics::~atGraphics()
{
  if (_pCurrent == this)
    _pCurrent = nullptr;
  m_pWindow->SetHardwareCtx(nullptr);
}

void* atGraphics::GetCtx() { return (void*)_pCurrent->m_pImpl->m_pDirectX; }

void atGraphics::Resize()
{
  switch (m_api)
  {
  case atGfxApi_DirectX:
  {
    m_pImpl->m_pDirectX->ResizeSwapChain(m_pWindow->Size());
    break;
  }
  case atGfxApi_OpenGL: // Open GL automatically resizes the swap chain
    break;
  }
}

bool atGraphics::Clear(const atVec4F &color, const float &depth)
{
  switch (m_api)
  {
  case atGfxApi_DirectX:
  {
    m_pImpl->m_pDirectX->GetContext()->ClearRenderTargetView(m_pImpl->m_pDirectX->GetBackbuffer(), &color[0]);
    m_pImpl->m_pDirectX->GetContext()->ClearDepthStencilView(m_pImpl->m_pDirectX->GetDepthBuffer(), D3D11_CLEAR_DEPTH, depth, 0xFF);
    break;
  }
  case atGfxApi_OpenGL:
    glClearColor(color.x, color.y, color.z, color.w);
    glClearDepth(depth);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    break;
  }

  return true;
}

bool atGraphics::Swap()
{
  switch (m_api)
  {
  case atGfxApi_DirectX:
    m_pImpl->m_pDirectX->GetSwapChain()->Present(m_pImpl->m_pDirectX->GetVsyncEnabled(), 0);
    break;
  case atGfxApi_OpenGL:
    m_pImpl->m_pOpenGL->Swap();
    break;
  }

  return true;
}

bool atGraphics::SetWindowed(const bool &windowed)
{
  switch (m_api)
  {
  case atGfxApi_DirectX:
  {
    m_pImpl->m_pDirectX->GetSwapChain()->SetFullscreenState(!windowed, nullptr);
    break;
  }
  case atGfxApi_OpenGL:
  {
    
  }
  }
  return false;
}

void atGraphics::SetCurrent(atGraphics *pContext) { _pCurrent = pContext; }

atGraphics* atGraphics::GetCurrent() { return _pCurrent; }

const atGraphicsAPI& atGraphics::GetAPI() const { return m_api; }

__atGfxImpl::__atGfxImpl(atWindow *pWindow, const atGraphicsAPI &api)
  : m_api(api)
{
  switch (m_api)
  {
  case atGfxApi_DirectX: m_pDirectX = atNew<atDirectX>(pWindow, true); break;
  case atGfxApi_OpenGL: m_pOpenGL = atNew<atOpenGL>(pWindow, true); break;
  default: m_pOpenGL = nullptr; break;
  }
}

__atGfxImpl::~__atGfxImpl()
{
  switch (m_api)
  {
  case atGfxApi_OpenGL: atDelete(m_pOpenGL); break;
  case atGfxApi_DirectX: atDelete(m_pDirectX); break;
  }
}
