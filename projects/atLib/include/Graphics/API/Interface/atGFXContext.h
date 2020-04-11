#ifndef atGFXApi_h__
#define atGFXApi_h__

#include "atMath.h"

enum atGraphicsAPI
{
  atGfxApi_None = -1,
  atGfxApi_OpenGL = 0,

#ifdef atPLATFORM_WIN32
  atGfxApi_DirectX = 1,
#endif

  atGfxApi_Count,
};

enum atGFX_PrimitiveType
{
  atGFX_PT_PointList,
  atGFX_PT_LineList,
  atGFX_PT_TriangleList,
  atGFX_PT_TriangleStrip,
  atGFX_PT_LineListAdj,
  atGFX_PT_TriangleListAdj,
};

enum atTexCoordMode
{
  atTCM_Unknown,
  atTCM_Wrap,           // D3D11_TEXTURE_ADDRESS_WRAP
  atTCM_Mirror,         // D3D11_TEXTURE_ADDRESS_MIRROR
  atTCM_MirrorOnce,     // D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
  atTCM_Clamp,          // D3D11_TEXTURE_ADDRESS_CLAMP
  atTCM_Border          // D3D11_TEXTURE_ADDRESS_BORDER
};

enum atComparison
{
  atComp_Unknown,
  atComp_Equal,         // D3D11_COMPARISON_EQUAL
  atComp_NotEqual,      // D3D11_COMPARISON_NOT_EQUAL
  atComp_Greater,       // D3D11_COMPARISON_GREATER
  atComp_Less,          // D3D11_COMPARISON_LESS
  atComp_GreaterEqual,  // D3D11_COMPARISON_GREATER_EQUAL
  atComp_LessEqual,     // D3D11_COMPARISON_LESS_EQUAL
  atComp_Always,        // D3D11_COMPARISON_ALWAYS
  atComp_Never         // D3D11_COMPARISON_NEVER
};

enum atSampleFilter
{
  atSF_Nearest,
  atSF_Linear,
  atSF_NearestMipmapNearest,
  atSF_LinearMipmapNearest,
  atSF_NearsetMipmapLinear,
  atSF_LinearMipmapLinear,
  atSF_Count,
};

class atWindow;
class atGFXContextState;

class atGFXContext
{
public:
  static atGFXContext* Create(const atGraphicsAPI& apiID, atWindow *pWindow, const bool &vsync);

  virtual atGFXContextState* GetState() = 0;
  virtual atGraphicsAPI API() = 0;

  // Commands
  virtual void DrawIndexed(int64_t nIndices, int64_t startLocation = 0, int64_t baseVertIndex = 0, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList, const atType &indicesType = atType_Uint32) = 0;
  virtual void Draw(int64_t nVerts, int64_t startLocation = 00, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList) = 0;

  // Swap chain
  virtual void Swap() = 0;
  virtual void SetWindowed(const bool &windowed) = 0;
  virtual void Resize(const atVec2I &size) = 0;

  // Back buffer
  virtual void ClearColour(const atVec4F &colour) = 0;
  virtual void ClearDepth(const float  &colour) = 0;
  virtual void ClearStencil() = 0;
};

#endif // atGFXApi_h__
