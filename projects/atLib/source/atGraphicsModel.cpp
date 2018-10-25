
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

void atGraphicsModel::Draw(const atMat4D &MVP)
{
  atMat4 mvpf = MVP;
  for (atRenderable &ro : m_mesh)
  {
    ro.SetChannel("mvp", mvpf.Transpose(), atRRT_Variable);
    ro.Draw();
  }
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
  atVector<atVector<uint32_t>> indices(mesh.m_materials.size(), atVector<uint32_t>());

  // Build vertex arrays
  for (const atMesh::Triangle &tri : mesh.m_triangles)
    for (int64_t v = 0; v < 3; ++v)
    {
      positions[tri.mat].push_back(mesh.m_positions[tri.verts[v].position]);
      texCoords[tri.mat].push_back(mesh.m_texCoords[tri.verts[v].texCoord]);
      normals[tri.mat].push_back(mesh.m_normals[tri.verts[v].normal]);
      colours[tri.mat].push_back(mesh.m_colors[tri.verts[v].color]);
    }


  atVector<atVector<atVec3F>> optNorm(mesh.m_materials.size(), atVector<atVec3F>());
  atVector<atVector<atVec4F>> optCol(mesh.m_materials.size(), atVector<atVec4F>());
  atVector<atVector<atVec3F>> optPos(mesh.m_materials.size(), atVector<atVec3F>());
  atVector<atVector<atVec2F>> optTex(mesh.m_materials.size(), atVector<atVec2F>());
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
      int64_t curIndex = optPos[m].size();
      if (m_vertices.TryAdd(vertData, curIndex))
      {
        indices[m].push_back((uint32_t)curIndex);
        optPos[m].push_back(positions[m][v]);
        optNorm[m].push_back(normals[m][v]);
        optTex[m].push_back(texCoords[m][v]);
        optCol[m].push_back(colours[m][v]);
      }
      else
        indices[m].push_back((uint32_t)m_vertices[vertData]);
    }

    m_mesh[m].SetShader("assets/shaders/color");
    m_mesh[m].SetChannel("samplerType", 0, atRRT_Sampler);
    m_mesh[m].SetChannel("COLOR", optCol[m], atRRT_VertexData);

    for (int64_t i = 0; i < mesh.m_materials[m].m_tAmbient.size(); ++i)
      m_mesh[m].SetChannel(atString("ambientTex") + i, mesh.m_materials[m].m_tAmbient[i].Path(), atRRT_Texture);

    for (int64_t i = 0; i < mesh.m_materials[m].m_tDiffuse.size(); ++i)
      m_mesh[m].SetChannel(atString("diffuseTex") + i, mesh.m_materials[m].m_tDiffuse[i].Path(), atRRT_Texture);

    for (int64_t i = 0; i < mesh.m_materials[m].m_tAlpha.size(); ++i)
      m_mesh[m].SetChannel(atString("alphaMap") + i, mesh.m_materials[m].m_tAlpha[i].Path(), atRRT_Texture);

    for (int64_t i = 0; i < mesh.m_materials[m].m_tSpecular.size(); ++i)
      m_mesh[m].SetChannel(atString("specularMap") + i, mesh.m_materials[m].m_tSpecular[i].Path(), atRRT_Texture);

    for (int64_t i = 0; i < mesh.m_materials[m].m_tSpecularHigh.size(); ++i)
      m_mesh[m].SetChannel(atString("specularHiMap") + i, mesh.m_materials[m].m_tSpecularHigh[i].Path(), atRRT_Texture);

    for (int64_t i = 0; i < mesh.m_materials[m].m_tDisplacement.size(); ++i)
      m_mesh[m].SetChannel(atString("displacementMap") + i, mesh.m_materials[m].m_tDisplacement[i].Path(), atRRT_Texture);

    for (int64_t i = 0; i < mesh.m_materials[m].m_tBump.size(); ++i)
      m_mesh[m].SetChannel(atString("bumpMap") + i, mesh.m_materials[m].m_tBump[i].Path(), atRRT_Texture);


    m_mesh[m].SetChannel("POSITION", optPos[m], atRRT_VertexData);
    m_mesh[m].SetChannel("TEXCOORD", optTex[m], atRRT_VertexData);
    m_mesh[m].SetChannel("idxBuffer", indices[m], atRRT_Indices);
  }
  return true;
}

bool atGraphicsModel::Import(const atFilename &filename)
{
  atMesh mesh;
  if (!atMeshReader::Read(filename, &mesh))
    return false;
  
  mesh.MakeValid();

  return Import(mesh);
}
