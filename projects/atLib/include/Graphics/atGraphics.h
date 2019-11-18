
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

#ifndef _atGraphics_h__
#define _atGraphics_h__

#include "atString.h"
#include "atGFXResource.h"

class atWindow;

class __atGfxImpl;

class atGraphics
{
  friend atWindow;

public:
  // Create a graphics context associated with a window
  atGraphics(atWindow *pWindow, const atGraphicsAPI &api = atGfxApi_DirectX);
  ~atGraphics();
  
  static void* GetCtx();

  // Set the current graphics context
  static void SetCurrent(atGraphics *pContext);

  // Get the current graphics context
  static atGraphics* GetCurrent();

  const atGraphicsAPI& GetAPI() const;

protected:
  void Resize();

  // Clear the window
  bool Clear(const atVec4F &color, const float &depth = 1.0f);

  // Display the window
  bool Swap();

  bool SetWindowed(const bool &windowed);

  atWindow *m_pWindow = nullptr;
  atGraphicsAPI m_api = atGfxApi_None;
  __atGfxImpl* m_pImpl = nullptr;
};

#endif // _atGraphics_h__
