
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
#include "atDirectX.h"
#include "atOpenGL.h"
#include "atWindow.h"

static atGraphics *_pCurrent = nullptr;

atGraphics::atGraphics(atWindow *pWindow, const atGraphicsAPI &api)
  : m_pWindow(pWindow)
  , m_api(api)
  , m_pContext(atGFXContext::Create(api, pWindow, true))
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

atGFXContext* atGraphics::GetCtx() { return _pCurrent->m_pContext; }

void atGraphics::Resize() { m_pContext->Resize(m_pWindow->Size()); }

bool atGraphics::Clear(const atVec4F &color, const float &depth)
{
  m_pContext->ClearColour(color);
  m_pContext->ClearDepth(depth);
  m_pContext->ClearStencil();
  return true;
}

bool atGraphics::Swap()
{
  m_pContext->Swap();
  return true;
}

bool atGraphics::SetWindowed(const bool &windowed)
{
  m_pContext->SetWindowed(windowed);
  return true;
}

void atGraphics::SetCurrent(atGraphics *pContext) { _pCurrent = pContext; }

atGraphics* atGraphics::GetCurrent() { return _pCurrent; }

const atGraphicsAPI& atGraphics::GetAPI() const { return m_api; }
