#include "atSceneAPI.h"

atScene *atSceneAPI::m_pScene = nullptr;

void atSceneAPI::SetScene(atScene *pScene)
{
  m_pScene = pScene;
}

int64_t atSceneAPI::Scene_AddNode(const char *name, int64_t parentID)
{
  return m_pScene->Add(atSceneNode(name), parentID);
}

void atSceneAPI::Scene_DeleteNode(int64_t node)
{
  m_pScene->Remove(node);
}

int64_t atSceneAPI::Scene_FindNodeByName(const char *name)
{
  atSceneNode *pNode = m_pScene->GetNode(name);
  return pNode ? m_pScene->GetNodeID(pNode) : atHierarchy_atRootNodeID;
}

atVector<int64_t> atSceneAPI::Scene_FindNodesByName(const char *name)
{
  atVector<atSceneNode*> nodeList = m_pScene->GetNodes(name);
  if (nodeList.size())
    return false;

  atVector<int64_t> nodeIDs;
  nodeIDs.reserve(nodeList.size());
  for (atSceneNode *pNode : nodeList)
    nodeIDs.push_back(m_pScene->GetNodeID(pNode));
  return nodeIDs;
}

bool atSceneAPI::Node_SetName(int64_t node, const char *newName)
{
  atSceneNode *pNode = m_pScene->Get(node);
  if (!pNode)
    return false;

  pNode->SetName(newName);
  return true;
}

const char* atSceneAPI::Node_GetName(int64_t node)
{
  static atString nameBuffer;

  atSceneNode *pNode = m_pScene->Get(node);
  if (!pNode)
    return "";

  nameBuffer = pNode->GetName();
  return nameBuffer.c_str();
}

bool atSceneAPI::Node_AddComponent(int64_t node, const atCoreComponentType &type, const char *name)
{
  atSceneNode *pNode = m_pScene->Get(node);
  if (!pNode)
    return false;
  
  switch (type)
  {
  case atCCT_Transform: pNode->AddComponent(atTransformComponent(name)); break;
  case atCCT_Camera: pNode->AddComponent(atCameraComponent(name)); break;
  case atCCT_Mesh: pNode->AddComponent(atMeshComponent(name)); break;
  case atCCT_Control: pNode->AddComponent(atControlComponent(name)); break;
  case atCCT_Script: pNode->AddComponent(atScriptComponent(name)); break;
  default: return false;
  }

  return true;
}

atSceneComponent* atSceneAPI::Node_GetComponent(int64_t node, const int64_t &index)
{
  atSceneNode *pNode = m_pScene->Get(node);
  if (!pNode)
    return nullptr;
  return pNode->GetComponent(index);
}

atSceneComponent* atSceneAPI::Node_GetComponentTyped(int64_t node, const atCoreComponentType &type, const int64_t &index)
{
  atSceneNode *pNode = m_pScene->Get(node);
  if (!pNode)
    return nullptr;
  return pNode->GetComponent(type, index);
}

int64_t atSceneAPI::Node_GetComponentCount(int64_t node)
{
  atSceneNode *pNode = m_pScene->Get(node);
  if (!pNode)
    return 0;

  return pNode->ComponentCount();
}

int64_t atSceneAPI::Node_GetComponentCountTyped(int64_t node, const atCoreComponentType &type)
{
  atSceneNode *pNode = m_pScene->Get(node);
  if (!pNode)
    return 0;

  return pNode->ComponentCount(type);
}

atMat4D atSceneAPI::Transform_GetTransform(atSceneComponent *pComponent)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return atMat4D::Identity();
  return ((atTransformComponent*)pComponent)->TransformMat();
}

atQuatD atSceneAPI::Transform_GetOrientation(atSceneComponent *pComponent)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return atQuatD::Identity();
  return ((atTransformComponent*)pComponent)->Orientation();
}

atVec3D atSceneAPI::Transform_GetTranslation(atSceneComponent *pComponent)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return atVec3D::zero();
  return ((atTransformComponent*)pComponent)->Translation();
}

atVec3D atSceneAPI::Transform_GetScale(atSceneComponent *pComponent)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return atVec3D::zero();
  return ((atTransformComponent*)pComponent)->Scale();
}

atVec3D atSceneAPI::Transform_GetYPR(atSceneComponent *pComponent)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return atVec3D::zero();
  return ((atTransformComponent*)pComponent)->RotationEuler();
}

atMat4D atSceneAPI::Transform_GetGlobalTransform(atSceneComponent *pComponent)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return atMat4D::Identity();
  return ((atTransformComponent*)pComponent)->GlobalTransformMat();
}

atVec3D atSceneAPI::Transform_GetGlobalTranslation(atSceneComponent *pComponent)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return atVec3D::zero();
  return ((atTransformComponent*)pComponent)->GlobalTranslation();
}

atVec3D atSceneAPI::Transform_GetGlobalScale(atSceneComponent *pComponent)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return atVec3D::zero();
  return ((atTransformComponent*)pComponent)->GlobalScale();
}

atVec3D atSceneAPI::Transform_GetGlobalYPR(atSceneComponent *pComponent)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return atVec3D::zero();
  return ((atTransformComponent*)pComponent)->GlobalRotationEuler();
}

void atSceneAPI::Transform_SetTranslation(atSceneComponent *pComponent, const atVec3D &translation)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return;
  return ((atTransformComponent*)pComponent)->SetTranslation(translation);
}

void atSceneAPI::Transform_SetOrientation(atSceneComponent *pComponent, const atQuatD &ori)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return;
  return ((atTransformComponent*)pComponent)->SetRotation(ori);
}

void atSceneAPI::Transform_SetYPR(atSceneComponent *pComponent, const atVec3D &rotation)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return;
  return ((atTransformComponent*)pComponent)->SetRotation(rotation);
}

void atSceneAPI::Transform_SetRotation(atSceneComponent *pComponent, const atMat4D &rotation)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return;
  return ((atTransformComponent*)pComponent)->SetRotation(rotation);
}

void atSceneAPI::Transform_SetScale(atSceneComponent *pComponent, const atVec3D &scale)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return;
  return ((atTransformComponent*)pComponent)->SetScale(scale);
}

void atSceneAPI::Transform_Translate(atSceneComponent *pComponent, const atVec3D &translation)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return;
  return ((atTransformComponent*)pComponent)->Translate(translation);
}

void atSceneAPI::Transform_RotateYPR(atSceneComponent *pComponent, const atVec3D &ypr)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return;
  return ((atTransformComponent*)pComponent)->Rotate(ypr);
}

void atSceneAPI::Transform_RotateOri(atSceneComponent *pComponent, const atQuatD &ori)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return;
  return ((atTransformComponent*)pComponent)->Rotate(ori);
}

void atSceneAPI::Transform_RotateRot(atSceneComponent *pComponent, const atMat4D &rot)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return;
  return ((atTransformComponent*)pComponent)->Rotate(rot);
}

void atSceneAPI::Transform_Scale(atSceneComponent *pComponent, const atVec3D &scale)
{
  if (!pComponent || pComponent->TypeID() != atCCT_Transform)
    return;
  return ((atTransformComponent*)pComponent)->Scale(scale);
}
