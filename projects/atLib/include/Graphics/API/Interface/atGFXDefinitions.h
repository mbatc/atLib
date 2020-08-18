#ifndef atGFXDefinitions_h__
#define atGFXDefinitions_h__

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

enum atBufferType
{
  atBT_VertexData,
  atBT_IndexData,
  atBT_ShaderData,
  atBT_Count,
};

enum atTextureType
{
  atTexture_None,
  atTexture_1D,
  atTexture_2D,
  atTexture_3D,
  atTexture_CubeMap,
  atTexture_Count,
};

enum atPipelineStage
{
  atPS_Vertex,
  atPS_Fragment,
  atPS_Geometry,
  atPS_Compute,
  atPS_Domain,
  atPS_Hull,
  atPS_Count,
  atPS_None,
};

#endif // atGFXDefinitions_h__
