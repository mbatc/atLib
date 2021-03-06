
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

#ifndef _atMesh_h__
#define _atMesh_h__

#include "atMath.h"
#include "atVector.h"
#include "atVector.h"
#include "atMaterial.h"
#include "atTriangle.h"
#include "atSkeleton.h"
#include "atAnimation.h"
#include "atVertexGroup.h"
#include "atAnimationGroup.h"
#include "atResourceHandler.h"

enum atVertexElement
{
  atVE_Position = 1,
  atVE_Normal = 1 << 1,
  atVE_TexCoord = 1 << 2,
  atVE_Tangent = 1 << 3,
  atVE_Bitangent = 1 << 4,
  atVE_Color = 1 << 5
};

class atMesh
{
public:
  bool Import(const atString &path);
  bool Export(const atString &path);

  struct Vertex
  {
    int64_t position = AT_INVALID_INDEX;
    int64_t normal = AT_INVALID_INDEX;
    int64_t color = AT_INVALID_INDEX;
    int64_t texCoord = AT_INVALID_INDEX;
    int64_t tangent = AT_INVALID_INDEX;
    int64_t bitanget = AT_INVALID_INDEX;
  };

  struct Triangle
  {
    int64_t mat;
    Vertex verts[3];
  };

  class VertexDeformer
  {
  public:
    int64_t boneID = -1;

	  atMat4D transform;
	  atMat4D inverseTransform;

    atVector<int64_t> vertices;
    atVector<double> weights;
  };

  class AnimTake
  {
  public:
    atHashMap<int64_t, int64_t> links;
    atAnimationGroup anim;

    atNanoSeconds startTime = atNanoSeconds(0ll);
    atNanoSeconds endTime = atNanoSeconds(1ll);
  };
  
  atMesh();
  atMesh(const atMesh &copy);
  atMesh(atMesh &&move);

  // These values will be used to fill in missing data when processing the mesh
  Vertex m_default;
  int64_t m_defaultMat = AT_INVALID_INDEX;

  // Fills missing data with default values
  // This should be called before using a mesh
  bool Validate();

  // Apply transforms to the mesh
  void SpatialTransform(const atMat4D &transform);
  void PositionTransform(const atMat4D &transform);
  void NormalTransform(const atMat4D &transform);

  // Attempt to make texture paths valid full paths
  void DiscoverTextures(const atString &initialPath = "");
  atString TryDiscoverFile(const atString &file, const atString &initialDir);

  // Generate Tangents and Bi-tangents
  void GenTangents();

  // Generate Normals
  void GenNormals();

  // If the dot product of two normals is < threshold they will not be blended
  void GenSmoothNormals(const double threshold = 0.6, const bool regenNormals = true);

  // Flip texture coordinates
  void FlipTextures(const bool u, const bool v);

  // Flip all normals
  void FlipNormals();

  // Combine a mesh
  void Combine(const atMesh &src);

  atVector<atTriangle<double>> GetTriangles() const;

  // Paths
  atString m_name;
  atString m_sourceFile;
  atString m_resourceDir;

  // Triangles
  atVector<Triangle> m_triangles;

  // Geometry
  atVector<atVec3D> m_positions;
  atVector<atVec3D> m_normals;
  atVector<atVec4D> m_colors;
  atVector<atVec2D> m_texCoords;
  atVector<atVec3D> m_tangents;
  atVector<atVec3D> m_binormals;

  // Bones
  atSkeleton m_skeleton;

  // Deformation vertex groups
  atVector<VertexDeformer> m_deformationGroups;

  // Materials
  atVector<atMaterial> m_materials;

  // Animations
  // Each group contains a collection of curves
  atVector<AnimTake> m_takes;

  const atMesh& operator=(const atMesh &rhs);
  const atMesh& operator=(atMesh &&rhs);
};

atTrivialStreamRead(atMesh::Triangle);
atTrivialStreamRead(atMesh::Vertex);
atTrivialStreamWrite(atMesh::Triangle);
atTrivialStreamWrite(atMesh::Vertex);

int64_t atStreamRead(atReadStream *pStream, atMesh *pData, const int64_t count);
int64_t atStreamWrite(atWriteStream *pStream, const atMesh *pData, const int64_t count);

namespace atResourceHandlers
{
  class MeshHandler : public atResourceHandler<atMesh>
  {
  public:
    MeshHandler() : atResourceHandler("Mesh") {}
    bool Load(const atObjectDescriptor &request, atMesh *pResource) override;
  };
};

#endif