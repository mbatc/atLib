
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

#include "atGraphics.h"
#include "atGFXResource.h"
#include "atDirectX.h"
#include "atOpenGL.h"
#include "atWindow.h"

static atGraphics *_pCurrent = nullptr;

atGraphics::atGraphics(atWindow *pWindow, const bool &vsync)
  : m_pWindow(pWindow)
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

atGraphics* atGraphics::Create(const atGraphicsAPI &apiID, atWindow *pWindow, const bool &vsync)
{
  switch (apiID)
  {
  case atGfxApi_DirectX: return atNew(atDirectX)(pWindow, vsync);
  case atGfxApi_OpenGL: return atNew(atOpenGL)(pWindow, vsync);
  }
  return nullptr;
}

bool atGraphics::Clear(const atVec4F &color, const float &depth)
{
  ClearColour(color);
  ClearDepth(depth);
  ClearStencil();
  return true;
}

void atGraphics::Release(atGFXResource *pResource)
{
  if (pResource && pResource->Release())
    atDelete(pResource);
  pResource = nullptr;
}

void atGraphics::SetCurrent(atGraphics *pContext)
{
  if (_pCurrent == pContext)
    return;
  _pCurrent = pContext;
}

atGraphics* atGraphics::GetCurrent() { return _pCurrent; }
