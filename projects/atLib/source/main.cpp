
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

#include <DirectXMath.h>
#include "atWindow.h"
#include "atInput.h"
#include "atRenderable.h"
#include <time.h>
#include <stdio.h>
#include "atRenderState.h"
#include "atCamera.h"
#include "atScan.h"
#include "atShaderParser.h"
#include "atGraphics.h"
#include "atHardwareTexture.h"
#include "atImage.h"
#include "atIntersects.h"
#include "atOBJParser.h"
#include "atMemoryWriter.h"

// NOTE: This file is used for testing

int main(int argc, char **argv)
{
  atUnused(argc, argv);
  atMesh mesh;
  atOBJReader::Read(atFilename("assets/test/models/suzan.obj"), &mesh);

  // Pre allocate memory
  atVector<atRenderable> renderList;
  renderList.resize(mesh.m_materials.size());
  atVector<atVector<atVec3F>> normals(mesh.m_materials.size(), atVector<atVec3F>());
  atVector<atVector<atVec4F>> colours(mesh.m_materials.size(), atVector<atVec4F>());
  atVector<atVector<atVec3F>> positions(mesh.m_materials.size(), atVector<atVec3F>());
  atVector<atVector<atVec2F>> texCoords(mesh.m_materials.size(), atVector<atVec2F>());
  atVector<atVector<uint32_t>> indices(mesh.m_materials.size(), atVector<uint32_t>());

  mesh.MakeValid();

  // Build vertex arrays
  for (atMesh::Triangle &tri : mesh.m_triangles)
    for (int64_t m = 0; m < 3; ++m)
    {
      positions[tri.mat].push_back(mesh.m_positions[tri.verts[m].position]);
      texCoords[tri.mat].push_back(mesh.m_texCoords[tri.verts[m].texCoord]);
      normals[tri.mat].push_back(mesh.m_normals[tri.verts[m].normal]);
      colours[tri.mat].push_back(mesh.m_colors[tri.verts[m].color]);
    }

  // Build Index Buffer
  for (int64_t m = 0; m < mesh.m_materials.size(); ++m)
  {
    atHashMap<atMemoryWriter, int64_t> m_vertices;
    atMemoryWriter vertData;
    for (int64_t v = 0; v < positions[m].size(); ++v)
    {
      vertData.Clear();
      vertData.Write(positions[m][v]);
      vertData.Write(texCoords[m][v]);
      vertData.Write(normals[m][v]);
      vertData.Write(colours[m][v]);
      if (!m_vertices.TryAdd(vertData, v))
      {
        indices[m].push_back((uint32_t)m_vertices[vertData]);
        positions[m].erase(v);
        texCoords[m].erase(v);
        normals[m].erase(v);
        colours[m].erase(v);
        --v;
      }
      else
        indices[m].push_back((uint32_t)v);
    }

    renderList[m].SetShader("assets/shaders/color");
    renderList[m].SetChannel("samplerType", 0, atRRT_Sampler);
    renderList[m].SetChannel("COLOR", colours[m], atRRT_VertexData);
    
    for (const atFilename &fn : mesh.m_materials[m].m_tDiffuse)
    {
      renderList[m].SetChannel("diffuseTexture", fn.Path(), atRRT_Texture);
      break;
    }

    renderList[m].SetChannel("POSITION", positions[m], atRRT_VertexData);
    renderList[m].SetChannel("TEXCOORD", texCoords[m], atRRT_VertexData);
    renderList[m].SetChannel("idxBuffer", indices[m], atRRT_Indices);
  }

  atWindow wnd("My window");
  atCamera cam(wnd, { 0,0, 5 });
  atRenderState::SetViewport(atVec4I(0, 0, wnd.GetSize()));
  
  float col = 66.f;
  atRenderState::Bind();
  while (atInput::Update())
  {
    atInput::LockMouse(true, wnd.GetSize() / 2);
    wnd.Clear(atVec4F(col / 255.f, col / 255.f, col / 255.f, 1.0f));

    for(atRenderable &ro : renderList)
      ro.Draw(cam.ProjectionMat() * cam.ViewMat());

    wnd.Swap();
    cam.Update(1);
  }
  return atWindow_GetResult();
}