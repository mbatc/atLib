
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

#ifndef _atGraphics_h__
#define _atGraphics_h__

#include "atString.h"
#include "atGFXDefinitions.h"

class atWindow;
class atGFXContextState;
class atGFXResource;
class atGPUBuffer;
class atTexture;
class atSampler;
class atShader;
class atRenderTarget;
class atProgram;

class atGraphics
{
  friend atWindow;

public:
  atGraphics(atWindow *pWindow, const bool &vsync);
  ~atGraphics();

  static atGraphics* Create(const atGraphicsAPI& apiID, atWindow *pWindow, const bool &vsync = true);

  virtual atGFXContextState* GetState() = 0;
  virtual atGraphicsAPI API() = 0;

  // Commands
  virtual void DrawIndexed(int64_t nIndices, int64_t startLocation = 0, int64_t baseVertIndex = 0, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList, const atType &indicesType = atType_Uint32) = 0;
  virtual void Draw(int64_t nVerts, int64_t startLocation = 0, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList) = 0;

  // Swap chain
  virtual void Swap() = 0;
  virtual void SetWindowed(const bool &windowed) = 0;
  virtual void Resize(const atVec2I &size) = 0;

  // Back buffer
  virtual void ClearColour(const atVec4F &colour) = 0;
  virtual void ClearDepth(const float  &colour) = 0;
  virtual void ClearStencil() = 0;

  virtual void BindContext() = 0;

  // Clear the window
  bool Clear(const atVec4F &color, const float &depth = 1.0f);

  // Create GFX objects that can be used with this graphics api
  virtual atSampler*      CreateSampler() = 0;
  virtual atShader*       CreateShader(const atPipelineStage &stage) = 0;
  virtual atProgram*      CreateProgram() = 0;
  virtual atGPUBuffer*    CreateBuffer(const atBufferType &bufferType = atBT_VertexData, const int64_t &size = 0) = 0;
  virtual atTexture*      CreateTexture(const atTextureType &texType = atTexture_2D) = 0;
  virtual atRenderTarget* CreateRenderTarget() = 0;
  static void Release(atGFXResource *pResource);

  // Set the current graphics context
  static void SetCurrent(atGraphics *pContext);

  // Get the current graphics context
  static atGraphics* GetCurrent();

protected:
  atWindow *m_pWindow = nullptr;
};

#include "atGraphics.h"

#endif // _atGraphics_h__
