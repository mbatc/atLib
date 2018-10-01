
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

#ifndef _atShader_h__
#define _atShader_h__

#include "atGraphics.h"
#include "atFilename.h"
#include "atHashMap.h"
#include "atShaderParser.h"

#define VERTEX_SHADER "vs_5_0"
#define PIXEL_SHADER "ps_5_0"
#define GEOM_SHADER "gs_5_0"
#define HULL_SHADER "hs_5_0"
#define DOMAIN_SHADER "ds_5_0"
#define COMPUTE_SHADER "cs_5_0"

class atShaderPool;

class atShader
{
  friend atShaderPool;

public:
  // Pass a shader file path without an extension. atShader will look for different types of shaders with the that name
  // The shaders name (identifier) will be set to [path]
  // Supported Extensions are:
  // ------------------------:
  //   *.vs (vertex shader)
  //   *.ps (pixel shader)
  //   *.gs (geometry shader)
  //   *.hs (hull shader)
  //   *.cs (compute shader)
  //   *.ds (domain shader)
  // ------------------------:
  atShader(const atString &path);
  ~atShader();

  void Reload();
  void Bind();

  bool SetHullShader(const atFilename &file);
  bool SetVertexShader(const atFilename &file);
  bool SetPixelShader(const atFilename &file);
  bool SetGeometryShader(const atFilename &file);
  bool SetComputeShader(const atFilename &file);
  bool SetDomainShader(const atFilename &file);
  const atString &GetName() const;

  // Managing Shader Inputs
  int64_t CreateInputLayout(const atVector<D3D11_INPUT_ELEMENT_DESC> &desc);
  bool SetResource(const int64_t loc, void *pData, const int64_t len);
  bool SetResource(const atString &name, void *pData, const int64_t len);
  int64_t GetResourceLoc(const atString &name);

private:
  void SetShaders(const atFilename &vert, const atFilename &pixel, const atFilename &hull, const atFilename &geom, const atFilename &compute, const atFilename &domain);
  bool SetShader(const atFilename &file, atShaderParser *pParser, atVector<uint8_t> *pByteCode, atFilename *pCurFile, void **pDXObject, const char *shaderType);
  void GenerateBuffers();
  void AddResources(const atShaderParser &parser, const atShaderType type);
  void UpdateResources();

  struct ResourceDesc
  {
    atShaderType shader;
    atShader_ResourceType type = atSRT_Buffer;
    atVector<int64_t> offsets;
    atVector<atString> vars;
    atVector<uint8_t> data;
    int64_t reg;
    void *pDXResource = nullptr;
  };

  bool m_regenBuffers;
  atVector<bool> m_bufDirty;
  atVector<ResourceDesc> m_resource;

  atVector<uint8_t> m_vsByteCode;
  atVector<uint8_t> m_psByteCode;
  atVector<uint8_t> m_gsByteCode;
  atVector<uint8_t> m_dsByteCode;
  atVector<uint8_t> m_csByteCode;
  atVector<uint8_t> m_hsByteCode;
  atHashMap<int64_t, ID3D11InputLayout*> m_layoutLookup;
  
  atFilename m_vertPath;
  atFilename m_pixelPath;
  atFilename m_hullPath;
  atFilename m_geomPath;
  atFilename m_domainPath;
  atFilename m_computePath;
  
  atShaderParser m_vsParser;
  atShaderParser m_psParser;
  atShaderParser m_gsParser;
  atShaderParser m_dsParser;
  atShaderParser m_csParser;
  atShaderParser m_hsParser;
 
  atString m_name;

  ID3D11HullShader *m_pHull;
  ID3D11PixelShader *m_pPixel;
  ID3D11VertexShader *m_pVert;
  ID3D11DomainShader *m_pDomain;
  ID3D11ComputeShader *m_pComp;
  ID3D11GeometryShader *m_pGeom;
  bool m_reload;
};

#endif
