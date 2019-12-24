
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

#ifndef _atMaterial_h__
#define _atMaterial_h__

#include "atMath.h"
#include "atHashMap.h"
#include "atFilename.h"
#include "atGFXPrgmInterface.h"

enum atMaterialProperty
{
  atMP_Alpha,
  atMP_Diffuse,
  atMP_Ambient,
  atMP_Specular,
  atMP_SpecularPower,
  atMP_SpecularHighlight,
  atMP_Bump,
  atMP_Normal,
  atMP_Displacement,
  atMP_Albedo,
  atMP_Metalness,
  atMP_Roughness,
  atMP_Occlusion,
  atMP_Count,
  atMP_Unknown,
};

enum atMaterialShader
{
  atMS_Coloured,
  atMS_Textured,
  atMS_Lambert,
  atMS_PBR,
  atMS_Count,
  atMS_Custom,
};

class atMaterial
{
protected:
  struct Layer
  {
    atHashMap<atString, double> m_values;
    atHashMap<atString, atVec4D> m_colours;
    atHashMap<atString, atString> m_textures;
  };

public:
  atMaterial();
  atMaterial(atMaterial &&o);
  atMaterial(const atMaterial &o);

  const atString& GetName() const;
  void SetName(const atString &name);

  bool HasValue(const atString &name, const int64_t &layer = 0) const;
  bool HasValue(const atMaterialProperty &name, const int64_t &layer = 0) const;

  bool HasColour(const atString &name, const int64_t &layer = 0) const;
  bool HasColour(const atMaterialProperty &name, const int64_t &layer = 0) const;

  bool HasTexture(const atString &name, const int64_t &layer = 0) const;
  bool HasTexture(const atMaterialProperty &name, const int64_t &layer = 0) const;

  bool HasShader(const atPipelineStage &stage) const;
  bool HasLayer(const int64_t &layer) const;
  
  void SetValue(const atString &name, const double &val, const int64_t &layer = 0);
  void SetValue(const atMaterialProperty &name, const double &val, const int64_t &layer = 0);

  void SetColour(const atString &name, const atVec4D &colour, const int64_t &layer = 0);
  void SetColour(const atMaterialProperty &name, const atVec4D &colour, const int64_t &layer = 0);
  
  void SetTexture(const atString &name, const atString &path, const int64_t &layer = 0);
  void SetTexture(const atMaterialProperty &name, const atString &path, const int64_t &layer = 0);
  
  void SetShader(const atMaterialShader &coreShader);
  void SetShader(const atPipelineStage &state, const atString &path);
  
  double GetValue(const atString &name, const int64_t &layer = 0) const;
  double GetValue(const atMaterialProperty &name, const int64_t &layer = 0) const;

  atVec4D GetColour(const atString &name, const int64_t &layer = 0) const;
  atVec4D GetColour(const atMaterialProperty &name, const int64_t &layer = 0) const;

  atString GetTexture(const atString &name, const int64_t &layer = 0) const;
  atString GetTexture(const atMaterialProperty &name, const int64_t &layer = 0) const;

  const atString& GetShader(const atPipelineStage &stage) const;
  
  void ClearValue(const atString &name, const int64_t &layer = 0);
  void ClearColour(const atString &name, const int64_t &layer = 0);
  void ClearTexture(const atString &name, const int64_t &layer = 0);
  void ClearShader(const atPipelineStage &state);

  int64_t LayerCount() const;

  atVector<atString> Values(const int64_t &layer = 0) const;
  atVector<atString> Colours(const int64_t &layer = 0) const;
  atVector<atString> Textures(const int64_t &layer = 0) const;

  // Property names
  static atString propertyName[atMP_Count];

  // Shader paths
  static atString shaderName[atMS_Count];
  
  static atMaterialProperty GetProperty(const atString &name);

  // Shader variable names
  static atString textureMapping[atMP_Count];
  static atString colourMapping[atMP_Count];
  static atString valueMapping[atMP_Count];

  friend int64_t atStreamRead(atReadStream *pStream, atMaterial *pData, const int64_t count);
  friend int64_t atStreamWrite(atWriteStream *pStream, const atMaterial *pData, const int64_t count);

  friend int64_t atStreamRead(atReadStream *pStream, Layer *pData, const int64_t count);
  friend int64_t atStreamWrite(atWriteStream *pStream, const Layer *pData, const int64_t count);

  atMaterial& operator=(atMaterial &&o);
  atMaterial& operator=(const atMaterial &o);

protected:
  Layer* GetOrAddLayer(const int64_t &index);
  Layer* GetLayer(const int64_t &index);
  const Layer* GetLayer(const int64_t &index) const;

  atString m_name;
  atMaterialShader m_shaderID = atMS_Lambert;
  
  atString m_shaders[atPS_Count];
  atVector<Layer> m_layer;
};


#endif