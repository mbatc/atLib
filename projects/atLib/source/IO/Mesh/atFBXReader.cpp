#include "atFBXParser.h"
#include "atFBXCommon.h"
#include "atHashMap.h"

struct _atFbxParseContext
{
  atMesh *pMesh;

  // Node ptr to bone index
  atHashMap<int64_t, int64_t> boneLookup;
  int64_t poseID = -1; // ID of the active pos
};

FbxAMatrix _GetGeometry(FbxNode *pNode)
{
  FbxDouble3 pivotTrans = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
  FbxDouble3 pivotRot = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
  FbxDouble3 pivotScl = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
  return FbxAMatrix(pivotTrans, pivotRot, pivotScl);
}

template<typename T> int64_t _GetMappedIndex(T *pElementArray, const int64_t &controlPoint, const int64_t &polyVert, const int64_t &polyIdx)
{
  if (!pElementArray)
    return -1;

  // Get the base index
  int64_t indirectIdx = -1;

  switch (pElementArray->GetMappingMode())
  {
  case FbxGeometryElement::eByPolygonVertex: indirectIdx = polyVert;     break;
  case FbxGeometryElement::eByControlPoint:  indirectIdx = controlPoint; break;
  case FbxGeometryElement::eByPolygon:       indirectIdx = polyIdx;      break;
  }

  int64_t directIdx = indirectIdx;

  // Convert indirect index to a direct index
  if (pElementArray->GetReferenceMode() != FbxGeometryElement::eDirect)
    directIdx = pElementArray->GetIndexArray().GetAt((int)indirectIdx);
  return directIdx;
}

template<typename Vec, typename T> Vec _ExtractElements(T *pElementArray, const int64_t &controlPoint, const int64_t &polyVert, const int64_t &polyIdx)
{
  typedef decltype(pElementArray->GetDirectArray().GetAt(0)) FbxElement; // Fbx element array item type

  int64_t directIdx = _GetMappedIndex(pElementArray, controlPoint, polyVert, polyIdx);

  // Get the item from the element array
  FbxElement item = pElementArray->GetDirectArray().GetAt((int)directIdx);
  Vec ret = { 0 };
  for (int64_t i = 0; i < Vec::ElementCount; ++i)
    ret[i] = item[(int)i];
  return ret;
}

static void _LoadTextures(const char *name, FbxSurfaceMaterial *pMaterial, atVector<atFilename> *pTextures)
{
  FbxProperty prop = pMaterial->FindProperty(name);
  for (int64_t i = 0; i < prop.GetSrcObjectCount<FbxTexture>(); ++i)
  {
    FbxFileTexture *pTex = prop.GetSrcObject<FbxFileTexture>((int)i);
    if (pTex)
      pTextures->push_back(pTex->GetFileName());
  }
}

static atVec4D _GetColour(const FbxPropertyT<FbxDouble3> &colour, const double &factor)
{
  FbxDouble3 val = colour.Get();
  return atVec4D(val[0], val[1], val[2], factor);
}

static atVec4D _GetColour(const FbxPropertyT<FbxDouble3> &colour, const FbxPropertyT<FbxDouble> &factor)
{
  return _GetColour(colour, factor.Get());
}

static atMaterial _LoadMaterial(FbxSurfaceMaterial *pMaterial)
{
  atMaterial mat;
  mat.m_name = pMaterial->GetName();

  bool isPhong = pMaterial->GetClassId() == FbxSurfacePhong::ClassId;
  bool isLambert = isPhong || pMaterial->GetClassId() == FbxSurfaceLambert::ClassId;

  if (isLambert)
  {
    FbxSurfaceLambert *pLambert = (FbxSurfaceLambert*)pMaterial;
    mat.m_cDiffuse = _GetColour(pLambert->Diffuse, pLambert->DiffuseFactor);
    mat.m_cAmbient = _GetColour(pLambert->Ambient, pLambert->AmbientFactor);
    mat.m_alpha = pLambert->TransparencyFactor.Get();

    _LoadTextures(FbxSurfaceMaterial::sDiffuse, pMaterial, &mat.m_tDiffuse);
    _LoadTextures(FbxSurfaceMaterial::sAmbientFactor, pMaterial, &mat.m_tAmbient);
    _LoadTextures(FbxSurfaceMaterial::sTransparencyFactor, pMaterial, &mat.m_tAlpha);
    _LoadTextures(FbxSurfaceMaterial::sDisplacementFactor, pMaterial, &mat.m_tDisplacement);
  }

  if (isPhong)
  {
    FbxSurfacePhong *pPhong = (FbxSurfacePhong*)pMaterial;
    mat.m_cSpecular = _GetColour(pPhong->Specular, pPhong->SpecularFactor);
    mat.m_specularPower = pPhong->SpecularFactor;

    _LoadTextures(FbxSurfaceMaterial::sSpecular, pMaterial, &mat.m_tSpecular);
  }

  return mat;
}

static bool _ParseDeformer(_atFbxParseContext *pCtx, FbxNode *pNode, FbxMesh *pFbxMesh, FbxSkin *pSkin)
{
  if (!pSkin)
    return false;

  FbxAMatrix geom = _GetGeometry(pNode);

  for (int64_t clusterIndex = 0; clusterIndex < (int64_t)pSkin->GetClusterCount(); ++clusterIndex)
  {
    FbxCluster *pCluster = pSkin->GetCluster((int)clusterIndex);
    int64_t *pBoneID = pCtx->boneLookup.TryGet((int64_t)pCluster->GetLink());
    if (!pBoneID)
      continue;

    if (pCtx->poseID == -1)
    {
      pCtx->poseID = pCtx->pMesh->m_skeleton.GetPoseCount();
      pCtx->pMesh->m_skeleton.AddPose(false);
    }
    atPose &pose = pCtx->pMesh->m_skeleton.GetPose(pCtx->poseID);

    atMesh::VertexDeformer deformer;
    FbxAMatrix clusterTransform;
    FbxAMatrix bonePoseTransform;
    FbxAMatrix associateMatrix;

    pCluster->GetTransformAssociateModelMatrix(associateMatrix);
    pCluster->GetTransformLinkMatrix(bonePoseTransform);
    pCluster->GetTransformMatrix(clusterTransform);

    // Set the cluster transform
    atAssign(deformer.transform, clusterTransform * geom);
    deformer.inverseTransform = deformer.transform.Inverse();

    // Set the bone bind position
    pose.SetTransform(*pBoneID, atAssign<atMat4D>(bonePoseTransform));

    deformer.boneID = *pBoneID;

    int *pIndices = pCluster->GetControlPointIndices();
    double *pWeights = pCluster->GetControlPointWeights();
    for (int64_t vert = 0; vert < (int64_t)pCluster->GetControlPointIndicesCount(); ++vert)
    {
      deformer.vertices.push_back(pIndices[vert]);
      deformer.weights.push_back(pWeights[vert]);
    }

    pCtx->pMesh->m_deformationGroups.push_back(deformer);
  }

  return true;
}

static bool _ParseDeformer(_atFbxParseContext *pCtx, FbxNode *pNode, FbxMesh *pFbxMesh, FbxVertexCacheDeformer *pCache)
{
  if (!pCache)
    return false;
  atUnused(pCtx, pNode, pFbxMesh, pCache);
  return false;
}

static bool _ParseDeformer(_atFbxParseContext *pCtx, FbxNode *pNode, FbxMesh *pFbxMesh, FbxBlendShape *pBlend)
{
  if (!pBlend)
    return false;
  atUnused(pCtx, pNode, pFbxMesh, pBlend);
  return false;
}

static void _ParseMesh(_atFbxParseContext *pCtx, FbxNode *pNode, FbxMesh *pFbxMesh)
{
  atMesh mesh;

  // Copy control points
  mesh.m_positions.reserve(pFbxMesh->GetControlPointsCount());
  for (const FbxVector4 &pos : atIterate(pFbxMesh->GetControlPoints(), pFbxMesh->GetControlPointsCount()))
    mesh.m_positions.push_back(atVec3D(pos[0], pos[1], pos[2]));

  // Get geometry element arrays
  FbxGeometryElementNormal      *pNormals     = pFbxMesh->GetElementNormal();
  FbxGeometryElementUV          *pUVs         = pFbxMesh->GetElementUV();
  FbxGeometryElementVertexColor *pVertexColor = pFbxMesh->GetElementVertexColor();
  FbxGeometryElementMaterial    *pMaterials   = pFbxMesh->GetElementMaterial();

  // Extract geometry elements
  int64_t polyVertIndex = 0;
  int64_t polyCount = pFbxMesh->GetPolygonCount();

  atVector<int64_t> materialLookup;
  mesh.m_materials.reserve(pNode->GetMaterialCount());
  materialLookup.resize(pNode->GetMaterialCount(), -1);

  int64_t defaultMaterial = -1;

  for (int64_t polygonIdx = 0; polygonIdx < polyCount; ++polygonIdx)
  {
    atMesh::Triangle tri;
    int64_t materialID = _GetMappedIndex(pMaterials, pFbxMesh->GetPolygonVertex((int)polygonIdx, 0), polyVertIndex, polygonIdx);

    if (materialID != -1)
    {
      int64_t &index = materialLookup[materialID];

      if (index == -1)
      { // Material has not been loaded yet
        index = mesh.m_materials.size();
        mesh.m_materials.push_back(_LoadMaterial(pNode->GetMaterial((int)materialID)));
      }

      tri.mat = index;
    }
    else
    {
      if (defaultMaterial == -1)
      {
        defaultMaterial = mesh.m_materials.size();
        mesh.m_materials.emplace_back();
      }
      tri.mat = defaultMaterial;
    }

    int64_t vertCount = pFbxMesh->GetPolygonSize((int)polygonIdx);
    for (int64_t vertIdx = 0; vertIdx < vertCount; ++vertIdx)
    {
      int64_t controlPointIdx = pFbxMesh->GetPolygonVertex((int)polygonIdx, (int)vertIdx);
      tri.verts[vertIdx].position = controlPointIdx;
      if (pNormals)
      {
        tri.verts[vertIdx].normal = mesh.m_normals.size();
        mesh.m_normals.push_back(_ExtractElements<atVec3D>(pNormals, controlPointIdx, polyVertIndex, polygonIdx));
      }

      if (pUVs)
      {
        tri.verts[vertIdx].texCoord = mesh.m_texCoords.size();
        mesh.m_texCoords.push_back(_ExtractElements<atVec2D>(pUVs, controlPointIdx, polyVertIndex, polygonIdx));
      }

      if (pVertexColor)
      {
        tri.verts[vertIdx].color = mesh.m_colors.size();
        mesh.m_colors.push_back(_ExtractElements<atVec4D>(pVertexColor, controlPointIdx, polyVertIndex, polygonIdx));
      }

      ++polyVertIndex;
    }

    mesh.m_triangles.push_back(tri);
  }

  for (int64_t i = 0; i < (int64_t)pFbxMesh->GetDeformerCount(); ++i)
  {
    if (!_ParseDeformer(pCtx, pNode, pFbxMesh, (FbxSkin*)pFbxMesh->GetDeformer((int)i, FbxDeformer::eSkin)))
      if (!_ParseDeformer(pCtx, pNode, pFbxMesh, (FbxVertexCacheDeformer*)pFbxMesh->GetDeformer((int)i, FbxDeformer::eVertexCache)))
        if (!_ParseDeformer(pCtx, pNode, pFbxMesh, (FbxBlendShape*)pFbxMesh->GetDeformer((int)i, FbxDeformer::eBlendShape)))
          continue;
  }

  FbxAMatrix pivot = _GetGeometry(pNode);
  mesh.SpatialTransform(atAssign<atMat4D>(pNode->EvaluateGlobalTransform() * pivot));
  atAssign(pCtx->pMesh->m_skeleton.Get(atHierarchy_RootNodeID).localTransform, pivot);
  pCtx->pMesh->Combine(mesh);
}

static void _ParseSkeleton(_atFbxParseContext *pCtx, FbxNode *pNode, FbxSkeleton *pFbxSkeleton)
{
  int64_t *pParentBone = pCtx->boneLookup.TryGet((int64_t)pNode->GetParent());
  atBone bone;
  bone.name = pNode->GetName();

  bool isRoot = pFbxSkeleton->IsSkeletonRoot();
  FbxAMatrix localTransform = pNode->EvaluateLocalTransform();
  FbxAMatrix globalTransform = pNode->EvaluateGlobalTransform();

  atAssign(bone.localTransform, localTransform);
  atAssign(bone.globalTransform, globalTransform);

  bone.modified = true;

  int64_t boneID = pCtx->pMesh->m_skeleton.Add(bone, pParentBone ? *pParentBone : atHierarchy_RootNodeID);
  pCtx->boneLookup.Add((int64_t)pNode, boneID);
}

static void _ParsePatch(_atFbxParseContext *pCtx, FbxNode *pNode, FbxPatch *pFbxPatch)
{

}

static void _ParseShape(_atFbxParseContext *pCtx, FbxNode *pNode, FbxShape *pFbxShape)
{

}

static void _ParseNurbs(_atFbxParseContext *pCtx, FbxNode *pNode, FbxNurbs *pFbxNurbs)
{

}

static void _ParseNode(_atFbxParseContext *pCtx, FbxNode *pNode, const FbxNodeAttribute::EType &type = FbxNodeAttribute::eUnknown)
{
  for (int64_t i = 0; i < (int64_t)pNode->GetNodeAttributeCount(); ++i)
  {
    FbxNodeAttribute *pAttribute = pNode->GetNodeAttributeByIndex((int)i);
    FbxNodeAttribute::EType attrType = pAttribute->GetAttributeType();

    if (type != FbxNodeAttribute::eUnknown && attrType != type)
      continue;

    switch (pAttribute->GetAttributeType())
    {
    case FbxNodeAttribute::eMesh:     _ParseMesh(pCtx, pNode, (FbxMesh*)pAttribute); break;
    case FbxNodeAttribute::eNurbs:    _ParseNurbs(pCtx, pNode, (FbxNurbs*)pAttribute); break;
    case FbxNodeAttribute::ePatch:    _ParsePatch(pCtx, pNode, (FbxPatch*)pAttribute); break;
    case FbxNodeAttribute::eShape:    _ParseShape(pCtx, pNode, (FbxShape*)pAttribute); break;
    case FbxNodeAttribute::eSkeleton: _ParseSkeleton(pCtx, pNode, (FbxSkeleton*)pAttribute); break;
    default: break;
    }
  }
    
  for (int64_t i = 0; i < (int64_t)pNode->GetChildCount(); ++i)
    _ParseNode(pCtx, pNode->GetChild((int)i), type);
}

static atAnimationCurve _ConvertCurve(FbxAnimCurve *pFbxCurve, const double &factor = 1)
{
  if (!pFbxCurve)
    return atAnimationCurve();

  atAnimationCurve converted;
  int keyCount = pFbxCurve->KeyGetCount();
  for (int i = 0; i < keyCount; ++i)
  {
    FbxAnimCurveKey fbxKey = pFbxCurve->KeyGet(i);
    atAnimationKey key;
    key.SetValue(fbxKey.GetValue() * factor);
    switch (fbxKey.GetInterpolation())
    {
    case FbxAnimCurveDef::eInterpolationConstant: key.SetInterpolation(atAnimationKey::Constant); break;
    case FbxAnimCurveDef::eInterpolationLinear: key.SetInterpolation(atAnimationKey::Linear); break;
    case FbxAnimCurveDef::eInterpolationCubic: key.SetInterpolation(atAnimationKey::Cubic); break;
    }

    converted.SetKey(atMilliSeconds(fbxKey.GetTime().GetMilliSeconds()), key);
  }

  return converted;
}

static void _ExtractCurveXYZ(FbxProperty *pProp, FbxAnimLayer *pLayer, atAnimationCurve *pCurveX = nullptr, atAnimationCurve *pCurveY = nullptr, atAnimationCurve *pCurveZ = nullptr, const double &factor = 1)
{
  if (pCurveX) *pCurveX = _ConvertCurve(pProp->GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_X), factor);
  if (pCurveY) *pCurveY = _ConvertCurve(pProp->GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Y), factor);
  if (pCurveZ) *pCurveZ = _ConvertCurve(pProp->GetCurve(pLayer, FBXSDK_CURVENODE_COMPONENT_Z), factor);
}

static void _ParseAnimations(_atFbxParseContext *pCtx, FbxScene *pScene)
{
  int numStacks = pScene->GetSrcObjectCount<FbxAnimStack>();
  for (int stackIdx = 0; stackIdx < numStacks; ++stackIdx)
  {
    FbxAnimStack *pStack = pScene->GetSrcObject<FbxAnimStack>(stackIdx);
    int numLayers = pStack->GetMemberCount<FbxAnimLayer>();

    atMesh::AnimTake take;
    take.anim.SetName(pStack->GetName());
    take.startTime = atMilliSeconds(pStack->GetLocalTimeSpan().GetStart().GetMilliSeconds());
    take.endTime = atMilliSeconds(pStack->GetLocalTimeSpan().GetStop().GetMilliSeconds());

    int64_t animGroupID = pCtx->pMesh->m_takes.size();

    for (int layerIdx = 0; layerIdx < numLayers; ++layerIdx)
    {
      // For each layer extract the animations for the bones
      FbxAnimLayer *pLayer = pStack->GetMember<FbxAnimLayer>(layerIdx);
      for (const atKeyValue<int64_t, int64_t> &kvp : pCtx->boneLookup)
      {
        int64_t animID = take.anim.AddAnimation();
        atAnimation *pAnimation = take.anim.GetAnimation(animID);

        FbxNode *pFbxBone = (FbxNode*)kvp.m_key;
        _ExtractCurveXYZ(&pFbxBone->LclTranslation, pLayer,
          pAnimation->GetCurve(atAnimationKeySet::atAnim_KS_TranslationX),
          pAnimation->GetCurve(atAnimationKeySet::atAnim_KS_TranslationY),
          pAnimation->GetCurve(atAnimationKeySet::atAnim_KS_TranslationZ));
        _ExtractCurveXYZ(&pFbxBone->LclRotation, pLayer,
          pAnimation->GetCurve(atAnimationKeySet::atAnim_KS_RotationX),
          pAnimation->GetCurve(atAnimationKeySet::atAnim_KS_RotationY),
          pAnimation->GetCurve(atAnimationKeySet::atAnim_KS_RotationZ), atPi / 180.0);
        _ExtractCurveXYZ(&pFbxBone->LclScaling, pLayer,
          pAnimation->GetCurve(atAnimationKeySet::atAnim_KS_ScaleX),
          pAnimation->GetCurve(atAnimationKeySet::atAnim_KS_ScaleY),
          pAnimation->GetCurve(atAnimationKeySet::atAnim_KS_ScaleZ));
        take.links.Add(kvp.m_val, animID);
      }
    }
    pCtx->pMesh->m_takes.push_back(take);
  }
}

bool atFBXReader::Read(const atFilename &file, atMesh *pMesh)
{
  // Clear the mesh
  *pMesh = atMesh();

  // Load the FBXScene
  atFBXCommon fbx;
  FbxScene *pScene = fbx.Import(file);

  // Triangulate the meshes
  FbxGeometryConverter converter(fbx.GetManager());
  converter.Triangulate(pScene, true);
  if (!pScene)
    return false;

  // Create a fresh FBX mesh parser context
  _atFbxParseContext ctx;
  ctx.pMesh = pMesh;

  // Only skeletons and meshes are supported so far
  _ParseNode(&ctx, pScene->GetRootNode(), FbxNodeAttribute::eSkeleton);
  _ParseNode(&ctx, pScene->GetRootNode(), FbxNodeAttribute::eMesh);

  // Extract animations after loading the geometry
  _ParseAnimations(&ctx, pScene);
  return true;
}
