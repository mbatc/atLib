
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

#include "atOBJParser.h"
#include "atMTLParser.h"
#include "atFile.h"
#include "atPrint.h"

static atString _WriteVertDef(const atMesh::Vertex &vert, const int64_t vFlags)
{
  atString ret;
  if ((vFlags & atVE_Position) > 0) ret += vert.position + 1ll;
  if ((vFlags & atVE_TexCoord) > 0 || (vFlags & atVE_Normal) > 0) ret += "/";
  if ((vFlags & atVE_TexCoord) > 0) ret += vert.texCoord + 1ll;  
  if ((vFlags & atVE_Normal) > 0)
  {
    ret += "/";
    ret += vert.normal + 1ll;
  }
  return ret;
}

static atString _WriteFaceDef(const atMesh::Triangle &tri, const int64_t vFlags)
{
  atString ret = "f";
  for (int64_t v = 0; v < 3; ++v)
    ret += " " + _WriteVertDef(tri.verts[v], vFlags);
  return ret;
}

bool atOBJWriter::Write(const atFilename &file, const atMesh &mesh) 
{ 
  atFile objFile;
  if (!objFile.Open(file, atFM_Write))
    return false;

  atString mtlFile = file.Name(false) + ".mtl";
  objFile.WriteText("# atLib OBJ File: " + file.Name(false));
  objFile.WriteText("\n# https://github.com/mbatc/atLib");
  objFile.WriteText("\n\nmtllib " + mtlFile);
  objFile.WriteText("\no " + (mesh.m_name.length() ? mesh.m_name : atString("default")) + "\n");
  for (const atVec3D &v : mesh.m_positions) objFile.WriteText("v " + atPrint::Vector(v) + "\n");
  for (const atVec2D &vt: mesh.m_texCoords) objFile.WriteText("vt " + atPrint::Vector(vt) + "\n");
  for (const atVec3D &vn : mesh.m_normals) objFile.WriteText("vn " + atPrint::Vector(vn) + "\n");
  const int64_t vFlags = (mesh.m_positions.size() > 0 * atVE_Position) | (mesh.m_texCoords.size() > 0 * atVE_TexCoord) | (mesh.m_normals.size() > 0 * atVE_Normal);
  for (int64_t m = 0; m < mesh.m_materials.size(); ++m)
  {
    objFile.WriteText("\nusemtl " + mesh.m_materials[m].m_name);
    objFile.WriteText("\ns off\n");
    for (const atMesh::Triangle &tri : mesh.m_triangles)
      if (tri.mat != m)
        continue;
      else
        objFile.WriteText(_WriteFaceDef(tri, vFlags) + "\n");
  }

  atMTLWriter::Write(file.Directory() + "/" + mtlFile, mesh.m_materials);
  return true; 
}