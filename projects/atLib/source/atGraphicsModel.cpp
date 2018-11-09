
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

#include "atGraphicsModel.h"
#include "atMemoryWriter.h"
#include "atMeshParser.h"

atGraphicsModel::atGraphicsModel() {}
atGraphicsModel::~atGraphicsModel() {}

atGraphicsModel::atGraphicsModel(const atMesh &mesh) { Import(mesh); }

atGraphicsModel::atGraphicsModel(const atFilename &filename) { Import(filename); }

atGraphicsModel::atGraphicsModel(const atGraphicsModel &copy) { for (const atRenderable &ro : copy.m_mesh) m_mesh.push_back(ro); }

atGraphicsModel::atGraphicsModel(atGraphicsModel &&move) { for (atRenderable &ro : move.m_mesh) m_mesh.push_back(std::move(ro)); }

void atGraphicsModel::Draw(const atMat4D &VP, const atMat4D &modelMat)
{
  atMat4 mvpf = VP * modelMat;
  atMat4 mf = modelMat;
  for (atRenderable &ro : m_mesh)
  {
    ro.SetChannel("mvp", mvpf.Transpose(), atRRT_Variable);
    ro.SetChannel("modelMat", mf.Transpose(), atRRT_Variable);
    ro.Draw();
  }
}

void atGraphicsModel::SetCamera(const atVec3F &pos)
{
  for (atRenderable &ro : m_mesh)
  {
    ro.SetChannel("camPos", pos, atRRT_Variable);
    ro.SetChannel("hasCamPos", (int32_t)1, atRRT_Variable);
  }
}

void atGraphicsModel::SetLighting(const atLight &light)
{
  for (atRenderable &ro : m_mesh)
  {
    ro.SetChannel("lightDiffuse0", atVec4F(light.m_diffuseColor), atRRT_Variable);
    ro.SetChannel("lightAmbient0", atVec4F(light.m_ambientColor), atRRT_Variable);
    ro.SetChannel("lightSpecular0", atVec4F(light.m_specularColor), atRRT_Variable);
    ro.SetChannel("lightDir0", atVec3F(light.m_direction), atRRT_Variable);
  }
  EnableLighting(true);
}

void atGraphicsModel::SetMaterials(const atVector<atMaterial>& materials, const int64_t start)
{
  for(int64_t i = 0; i < materials.size() && i + start < m_mesh.size(); ++i)
  {
    const atMaterial &mat = materials[i];
    atRenderable &mesh = m_mesh[start + i];

    bool hasAmbientMap = mat.m_tAmbient.size() > 0;
    bool hasDiffuseMap = mat.m_tDiffuse.size() > 0;
    bool hasAlphaMap = mat.m_tAlpha.size() > 0;
    bool hasSpecularMap = mat.m_tSpecular.size() > 0;
    bool hasSpecularHiMap = mat.m_tSpecularHigh.size() > 0;
    bool hasDisplacementMap = mat.m_tDisplacement.size() > 0;
    bool hasBumpMap = mat.m_tBump.size() > 0;

    for (int64_t i = 0; i < mat.m_tAmbient.size(); ++i)
      mesh.SetChannel(atString("ambientMap") + i, mat.m_tAmbient[i].Path(), atRRT_Texture);

    for (int64_t i = 0; i < mat.m_tDiffuse.size(); ++i)
      mesh.SetChannel(atString("diffuseMap") + i, mat.m_tDiffuse[i].Path(), atRRT_Texture);

    for (int64_t i = 0; i < mat.m_tAlpha.size(); ++i)
      mesh.SetChannel(atString("alphaMap") + i, mat.m_tAlpha[i].Path(), atRRT_Texture);

    for (int64_t i = 0; i < mat.m_tSpecular.size(); ++i)
      mesh.SetChannel(atString("specularMap") + i, mat.m_tSpecular[i].Path(), atRRT_Texture);

    for (int64_t i = 0; i < mat.m_tSpecularHigh.size(); ++i)
      mesh.SetChannel(atString("specularHiMap") + i, mat.m_tSpecularHigh[i].Path(), atRRT_Texture);

    for (int64_t i = 0; i < mat.m_tDisplacement.size(); ++i)
      mesh.SetChannel(atString("displacementMap") + i, mat.m_tDisplacement[i].Path(), atRRT_Texture);

    for (int64_t i = 0; i < mat.m_tBump.size(); ++i)
      mesh.SetChannel(atString("bumpMap") + i, mat.m_tBump[i].Path(), atRRT_Texture);

    mesh.SetChannel("diffuseColour0", atVec4F(mat.m_cDiffuse), atRRT_Variable);
    mesh.SetChannel("ambientColour0", atVec4F(mat.m_cAmbient), atRRT_Variable);
    mesh.SetChannel("specularColour0", atVec4F(mat.m_cSpecular), atRRT_Variable);
    mesh.SetChannel("alpha0", (float)mat.m_alpha, atRRT_Variable);
    mesh.SetChannel("specularPower0", (float)mat.m_specularPower, atRRT_Variable);

    mesh.SetChannel("hasAmbientMap", (int32_t)hasAmbientMap, atRRT_Variable);
    mesh.SetChannel("hasDiffuseMap", (int32_t)hasDiffuseMap, atRRT_Variable);
    mesh.SetChannel("hasAlphaMap", (int32_t)hasAlphaMap, atRRT_Variable);
    mesh.SetChannel("hasSpecularMap", (int32_t)hasSpecularMap, atRRT_Variable);
    mesh.SetChannel("hasSpecularHiMap", (int32_t)hasSpecularHiMap, atRRT_Variable);
    mesh.SetChannel("hasDisplacementMap", (int32_t)hasDisplacementMap, atRRT_Variable);
    mesh.SetChannel("hasBumpMap", (int32_t)hasBumpMap, atRRT_Variable);
  }
}

void atGraphicsModel::EnableLighting(const bool enable)
{
  for (atRenderable &ro : m_mesh)
    ro.SetChannel("hasLighting", (int32_t)enable, atRRT_Variable);
}

bool atGraphicsModel::Import(const atMesh &mesh)
{
  // Pre allocate memory
  m_mesh.clear();
  m_mesh.resize(mesh.m_materials.size());

  atVector<atVector<atVec3F>> normals(mesh.m_materials.size(), atVector<atVec3F>());
  atVector<atVector<atVec4F>> colours(mesh.m_materials.size(), atVector<atVec4F>());
  atVector<atVector<atVec3F>> positions(mesh.m_materials.size(), atVector<atVec3F>());
  atVector<atVector<atVec2F>> texCoords(mesh.m_materials.size(), atVector<atVec2F>());
  atVector<atVector<atVec3F>> tangents(mesh.m_materials.size(), atVector<atVec3F>());
  atVector<atVector<atVec3F>> binormal(mesh.m_materials.size(), atVector<atVec3F>());
  atVector<atVector<uint32_t>> indices(mesh.m_materials.size(), atVector<uint32_t>());

  // Build vertex arrays
  for (const atMesh::Triangle &tri : mesh.m_triangles)
    for (int64_t v = 0; v < 3; ++v)
    {
      positions[tri.mat].push_back(mesh.m_positions[tri.verts[v].position]);
      texCoords[tri.mat].push_back(mesh.m_texCoords[tri.verts[v].texCoord]);
      normals[tri.mat].push_back(mesh.m_normals[tri.verts[v].normal]);
      colours[tri.mat].push_back(mesh.m_colors[tri.verts[v].color]);
      tangents[tri.mat].push_back(mesh.m_tangents[tri.verts[v].tangent]);
      binormal[tri.mat].push_back(mesh.m_binormals[tri.verts[v].bitanget]);
    }


  atVector<atVector<atVec3F>> optNorm(mesh.m_materials.size(), atVector<atVec3F>());
  atVector<atVector<atVec4F>> optCol(mesh.m_materials.size(), atVector<atVec4F>());
  atVector<atVector<atVec3F>> optPos(mesh.m_materials.size(), atVector<atVec3F>());
  atVector<atVector<atVec2F>> optTex(mesh.m_materials.size(), atVector<atVec2F>());
  atVector<atVector<atVec3F>> optTan(mesh.m_materials.size(), atVector<atVec3F>());
  atVector<atVector<atVec3F>> optBiNorm(mesh.m_materials.size(), atVector<atVec3F>());
  atVector<atVector<uint32_t>> optInd(mesh.m_materials.size(), atVector<uint32_t>());

  // Build Index Buffer
  for (int64_t m = 0; m < mesh.m_materials.size(); ++m)
  {
    // Create all buckets needed
    atHashMap<atMemoryWriter, int64_t> m_vertices(mesh.m_triangles.size() * 3 / 16);
    atMemoryWriter vertData;
    for (int64_t v = 0; v < positions[m].size(); ++v)
    {
      vertData.Clear();
      vertData.Write(positions[m][v]);
      vertData.Write(texCoords[m][v]);
      vertData.Write(normals[m][v]);
      vertData.Write(colours[m][v]);
      vertData.Write(tangents[m][v]);
      vertData.Write(binormal[m][v]);

      int64_t curIndex = optPos[m].size();
      if (m_vertices.TryAdd(vertData, curIndex))
      {
        indices[m].push_back((uint32_t)curIndex);
        optPos[m].push_back(positions[m][v]);
        optNorm[m].push_back(normals[m][v]);
        optTex[m].push_back(texCoords[m][v]);
        optCol[m].push_back(colours[m][v]);
        optTan[m].push_back(tangents[m][v]);
        optBiNorm[m].push_back(binormal[m][v]);
      }
      else
        indices[m].push_back((uint32_t)m_vertices[vertData]);
    }

    m_mesh[m].SetShader("assets/shaders/uber");
    m_mesh[m].SetChannel("samplerType", 0, atRRT_Sampler);
    m_mesh[m].SetChannel("COLOR", optCol[m], atRRT_VertexData);
    m_mesh[m].SetChannel("POSITION", optPos[m], atRRT_VertexData);
    m_mesh[m].SetChannel("TEXCOORD", optTex[m], atRRT_VertexData);
    m_mesh[m].SetChannel("NORMAL", optNorm[m], atRRT_VertexData);
    m_mesh[m].SetChannel("TANGENT", optTan[m], atRRT_VertexData);
    m_mesh[m].SetChannel("BINORMAL", optBiNorm[m], atRRT_VertexData);
    m_mesh[m].SetChannel("idxBuffer", indices[m], atRRT_Indices);
  }
  SetMaterials(mesh.m_materials);
  SetLighting(atLight());
  EnableLighting(false);
  
  return true;
}

bool atGraphicsModel::Import(const atFilename &filename)
{
  atMesh mesh;
  if (!atMeshReader::Read(filename, &mesh))
    return false;
  
  mesh.MakeValid();
  mesh.FlipTextures(false, true);
  mesh.DiscoverTextures();

  return Import(mesh);
}
