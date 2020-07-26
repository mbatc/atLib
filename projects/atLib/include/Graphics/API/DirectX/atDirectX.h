#ifndef atDirectX_h__
#define atDirectX_h__

#include "atString.h"
#include "atGraphics.h"

class atWindow;
class atDirectXState;

class atDirectX : public atGraphics
{
public:
  atDirectX(atWindow *pWindow, const bool &vsyncEnabled = true);
  ~atDirectX();

  void Swap() override;
  void Resize(const atVec2I &size) override;
  void ClearColour(const atVec4F &colour) override;
  void ClearDepth(const float &colour) override;
  void ClearStencil() override;
  void SetWindowed(const bool &windowed) override;
  
  atGFXContextState* GetState();

  atGPUBuffer*    CreateBuffer(const atBufferType &type) override;
  atTexture*      CreateTexture(const atTextureType &type) override;
  atSampler*      CreateSampler() override;
  atShader*       CreateShader(const atString &src, const atPipelineStage &stage) override;
  atProgram*      CreateProgram() override;
  atRenderTarget* CreateRenderTarget() override;

  void* GetSwapChain();
  void* GetBackbuffer();
  void* GetDepthBuffer();
  void* GetOutputDisplay();
  void* GetFactory();
  void* GetDisplayAdapter();
  void* GetDevice();
  void* GetContext();

  atString GetAdapterDescription();
  int64_t GetFeatureLevel();
  int64_t GetCreateFlags();

  bool GetVsyncEnabled() const;
  int64_t RefreshRateNumerator(const bool vsync);
  int64_t RefreshRateDenominator(const bool vsync);
  
  int64_t GFXMemorySize();

  // Execute a draw command using the current OpenGL state
  void DrawIndexed(int64_t nIndices, int64_t startLocation = 0, int64_t baseVertIndex = 0, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList, const atType &indicesType = atType_Uint32) override;
  void Draw(int64_t nVerts, int64_t startLocation = 00, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList) override;

  void Shutdown();

  atGraphicsAPI API() override;

  template<typename T> static void SafeRelease(T *&pBuffer) { if (pBuffer) pBuffer->Release(); pBuffer = nullptr; }

protected:
  void CreateSwapChain(atWindow *pWindow, const bool &vsyncEnabled);
  void CreateDeviceAndContext();
  void CreateOutput();
  void CreateAdapter();
  void CreateFactory();
  void CreateBackBufferView();
  void CreateDepthBuffer(const atVec2I &size);

  void *m_pImpl = nullptr;

  atDirectXState *m_pState = nullptr;
};

#endif // atDirectX_h__
