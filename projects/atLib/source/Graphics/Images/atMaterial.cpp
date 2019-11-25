
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

#include "atMaterial.h"
#include "atFile.h"

atString atMaterial::propertyName[atMP_Count] =
{
  "alpha",              // atMP_Alpha
  "diffuse",            // atMP_Diffuse
  "ambient",            // atMP_Ambient
  "specular",           // atMP_Specular
  "specularPower",      // atMP_SpecularPower
  "specularHighlight",  // atMP_SpecularHighlight
  "bump",               // atMP_Bump
  "normal",             // atMP_Normal
  "displacement",       // atMP_Displacement
  "albedo",             // atMP_Albedo
  "metalness",          // atMP_Metalness
  "roughness",          // atMP_Roughness
  "occlusion"           // atMP_Occlusion
};

atString atMaterial::shaderName[atMS_Count] =
{
  "meshColoured",       // atMS_Coloured
  "meshTextured",       // atMS_Textured
  "meshLambert",        // atMS_Lambert
  "meshPBR"             // atMS_PBR
};

atString atMaterial::textureMapping[atMP_Count] = 
{
  "alphaMap",           // atMP_Alpha
  "diffuseMap",         // atMP_Diffuse
  "ambientMap",         // atMP_Ambient
  "specularMap",        // atMP_Specular
  "specularPowMap",     // atMP_SpecularPower
  "specularHiMap",      // atMP_SpecularHighlight
  "bumpMap",            // atMP_Bump
  "normalMap",          // atMP_Normal
  "displacementMap",    // atMP_Displacement
  "albedoMap",          // atMP_Albedo
  "metalnessMap",       // atMP_Metalness
  "roughnessMap",       // atMP_Roughness
  "occlusionMap"        // atMP_Occlusion
};

atString atMaterial::colourMapping[atMP_Count] =
{
  "",                   // atMP_Alpha
  "diffuseCol",         // atMP_Diffuse
  "ambientCol",         // atMP_Ambient
  "specularCol",        // atMP_Specular
  "",                   // atMP_SpecularPower
  "specularHiCol",      // atMP_SpecularHighlight
  "",                   // atMP_Bump
  "",                   // atMP_Normal
  "",                   // atMP_Displacement
  "albedo",             // atMP_Albedo
  "",                   // atMP_Metalness
  "",                   // atMP_Roughness
  ""                    // atMP_Occlusion
};

atString atMaterial::valueMapping[atMP_Count] =
{
  "alpha",              // atMP_Alpha
  "diffuseFactor",      // atMP_Diffuse
  "ambientFactor",      // atMP_Ambient
  "specularFactor",     // atMP_Specular
  "specularPow",        // atMP_SpecularPower
  "specularHiFactor",   // atMP_SpecularHighlight
  "bumpFactor",         // atMP_Bump
  "normalFactor",       // atMP_Normal
  "displacementFactor", // atMP_Displacement
  "albedoFactor",       // atMP_Albedo
  "metalness",          // atMP_Metalness
  "roughness",          // atMP_Roughness
  "occlusion"           // atMP_Occlusion
};


atMaterial::atMaterial()
{
  SetValue(atMP_Alpha, 1.0);
  SetColour(atMP_Diffuse, atVec4D::one());
  SetColour(atMP_Ambient, atVec4D::zero());
  SetColour(atMP_Specular, atVec4D::zero());
  SetShader(atMS_Coloured);
}

atMaterial::atMaterial(atMaterial &&o) { *this = std::move(o); }
atMaterial::atMaterial(const atMaterial &o) { *this = o; }

atMaterial& atMaterial::operator=(atMaterial &&o)
{
  m_name = std::move(o.m_name);
  m_shaderID = std::move(o.m_shaderID);
  for (int64_t i = 0; i < atArraySize(m_layer); ++i)
    m_layer = std::move(o.m_layer);
  m_layer = std::move(o.m_layer);
  return *this;
}

atMaterial& atMaterial::operator=(const atMaterial &o)
{
  m_name = o.m_name;
  m_shaderID = o.m_shaderID;
  for (int64_t i = 0; i < atArraySize(m_layer); ++i)
    m_layer = o.m_layer;
  m_layer = o.m_layer;
  return *this;
}

atMaterial::Layer* atMaterial::GetOrAddLayer(const int64_t &index)
{
  m_layer.resize(atMax(index + 1, m_layer.size()));
  return &m_layer[index];
}

void atMaterial::SetValue(const atString &name, const double &val, const int64_t &layer)
{
  if (name.length() == 0)
    return;

  Layer *pLayer = GetOrAddLayer(layer);
  if (!pLayer->m_values.TryAdd(name, val))
    pLayer->m_values[name] = val;
}

void atMaterial::ClearValue(const atString &name, const int64_t &layer)
{
  if (HasLayer(layer))
    GetLayer(layer)->m_values.Remove(name);
}

void atMaterial::SetColour(const atString &name, const atVec4D &colour, const int64_t &layer)
{
  if (name.length() == 0)
    return;

  Layer *pLayer = GetOrAddLayer(layer);
  if (!pLayer->m_colours.TryAdd(name, colour))
    pLayer->m_colours[name] = colour;
}

void atMaterial::ClearColour(const atString &name, const int64_t &layer)
{
  if (HasLayer(layer))
    GetLayer(layer)->m_colours.Remove(name);
}

void atMaterial::SetTexture(const atString &name, const atString &path, const int64_t &layer)
{
  if (name.length() == 0)
    return;

  if (path.length() == 0)
  {
    ClearTexture(name, layer);
    return;
  }

  Layer *pLayer = GetOrAddLayer(layer);
  if (!pLayer->m_textures.TryAdd(name, path))
    pLayer->m_textures[name] = path;
}

void atMaterial::ClearTexture(const atString &name, const int64_t &layer)
{
  if (HasLayer(layer))
    GetLayer(layer)->m_textures.Remove(name);
}

void atMaterial::SetShader(const atMaterialShader &coreShader)
{
  for (int64_t i = 0; i < atPS_Count; ++i)
    ClearShader(atPipelineStage(i));
  SetShader(atPS_Vertex, shaderName[coreShader] + ".vs");
  SetShader(atPS_Fragment, shaderName[coreShader] + ".ps");
}

bool atMaterial::HasTexture(const atString &name, const int64_t &layer) const
{
  const Layer *pLayer = GetLayer(layer);
  const atString *pTex = pLayer->m_textures.TryGet(name);
  return pTex ? pTex->length() > 0 : false;
}

atMaterial::Layer* atMaterial::GetLayer(const int64_t &index) { return HasLayer(index) ? &m_layer[index] : nullptr; }
const atMaterial::Layer* atMaterial::GetLayer(const int64_t &index) const { return HasLayer(index) ? &m_layer[index] : nullptr; }

const atString& atMaterial::GetName() const { return m_name; }
double atMaterial::GetValue(const atString &name, const int64_t &layer) const { return HasValue(name, layer) ? GetLayer(layer)->m_values[name] : 0.0; }
atVec4D atMaterial::GetColour(const atString &name, const int64_t &layer) const { return HasValue(name, layer) ? GetLayer(layer)->m_colours[name] : 0.0; }
atString atMaterial::GetTexture(const atString &name, const int64_t &layer) const { return HasTexture(name, layer) ? GetLayer(layer)->m_textures[name] : ""; }
double atMaterial::GetValue(const atMaterialProperty &name, const int64_t &layer) const { return GetValue(propertyName[name], layer); }
atVec4D atMaterial::GetColour(const atMaterialProperty &name, const int64_t &layer) const { return GetColour(propertyName[name], layer); }
atString atMaterial::GetTexture(const atMaterialProperty &name, const int64_t &layer) const { return GetTexture(propertyName[name], layer); }

void atMaterial::SetName(const atString &name) { m_name = name; }
void atMaterial::SetShader(const atPipelineStage &stage, const atString &path) { m_shaders[stage] = path; }
void atMaterial::SetValue(const atMaterialProperty &name, const double &val, const int64_t &layer) { SetValue(propertyName[name], val, layer); }
void atMaterial::SetColour(const atMaterialProperty &name, const atVec4D &colour, const int64_t &layer) { SetColour(propertyName[name], colour, layer); }
void atMaterial::SetTexture(const atMaterialProperty &name, const atString &path, const int64_t &layer) { SetTexture(propertyName[name], path, layer); }

bool atMaterial::HasValue(const atString &name, const int64_t &layer) const { return HasLayer(layer) ? GetLayer(layer)->m_values.Contains(name) : false; }
bool atMaterial::HasColour(const atString &name, const int64_t &layer) const { return HasLayer(layer) ? GetLayer(layer)->m_colours.Contains(name) : false; }
bool atMaterial::HasValue(const atMaterialProperty &name, const int64_t &layer) const { return HasValue(propertyName[name], layer); }
bool atMaterial::HasColour(const atMaterialProperty &name, const int64_t &layer) const { return HasColour(propertyName[name], layer); }
bool atMaterial::HasShader(const atPipelineStage &stage) const { return GetShader(stage).length() > 0; }
bool atMaterial::HasTexture(const atMaterialProperty &name, const int64_t &layer) const { return HasTexture(propertyName[name], layer); }

void atMaterial::ClearShader(const atPipelineStage &stage) { SetShader(stage, ""); }
const atString& atMaterial::GetShader(const atPipelineStage &stage) const { return m_shaders[stage]; }

int64_t atMaterial::LayerCount() const { return m_layer.size(); }
bool atMaterial::HasLayer(const int64_t &layer) const { return layer >= 0 && layer < LayerCount(); }

atVector<atString> atMaterial::Values(const int64_t &layer) const { return HasLayer(layer) ? GetLayer(layer)->m_values.GetKeys() : atVector<atString>(); }
atVector<atString> atMaterial::Colours(const int64_t &layer) const { return HasLayer(layer) ? GetLayer(layer)->m_colours.GetKeys() : atVector<atString>(); }
atVector<atString> atMaterial::Textures(const int64_t &layer) const { return HasLayer(layer) ? GetLayer(layer)->m_textures.GetKeys() : atVector<atString>(); }

atMaterialProperty atMaterial::GetProperty(const atString &name)
{
  atString lwrName = name.to_lower();
  for (int64_t i = 0; i < atMP_Count; ++i)
    if (lwrName == propertyName[i])
      return atMaterialProperty(i);
  return atMP_Unknown;
}

int64_t atStreamRead(atReadStream *pStream, atMaterial *pData, const int64_t count)
{
  int64_t size = 0;
  for (atMaterial &mat : atIterate(pData, count))
  {
    size += atStreamRead(pStream, &mat.m_name, 1);
    size += atStreamRead(pStream, &mat.m_shaderID, 1);
    size += atStreamRead(pStream, &mat.m_layer, 1);
    for (int64_t i = 0; i < atPS_Count; ++i)
      size += atStreamRead(pStream, &mat.m_shaders[i], 1);
  }
  return size;
}

int64_t atStreamWrite(atWriteStream *pStream, const atMaterial *pData, const int64_t count)
{
  int64_t size = 0;
  for (const atMaterial &mat : atIterate(pData, count))
  {
    size += atStreamWrite(pStream, &mat.m_name, 1);
    size += atStreamWrite(pStream, &mat.m_shaderID, 1);
    size += atStreamWrite(pStream, &mat.m_layer, 1);
    for (int64_t i = 0; i < atPS_Count; ++i)
      size += atStreamWrite(pStream, &mat.m_shaders[i], 1);
  }
  return size;
}

int64_t atStreamRead(atReadStream *pStream, atMaterial::Layer *pData, const int64_t count)
{
  int64_t size = 0;
  for (atMaterial::Layer &layer : atIterate(pData, count))
  {
    size += atStreamRead(pStream, &layer.m_values, 1);
    size += atStreamRead(pStream, &layer.m_colours, 1);
    size += atStreamRead(pStream, &layer.m_textures, 1);
  }
  return size;
}

int64_t atStreamWrite(atWriteStream *pStream, const atMaterial::Layer *pData, const int64_t count)
{
  int64_t size = 0;
  for (const atMaterial::Layer &layer : atIterate(pData, count))
  {
    size += atStreamWrite(pStream, &layer.m_values, 1);
    size += atStreamWrite(pStream, &layer.m_colours, 1);
    size += atStreamWrite(pStream, &layer.m_textures, 1);
  }
  return size;
}
