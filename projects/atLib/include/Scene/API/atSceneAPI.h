#ifndef atSceneAPI_h__
#define atSceneAPI_h__

#include "atScene.h"

class atSceneAPI
{
public:
  // Setting up the API

  // Sets the scene that calls to the API will influence.
  // This can be changed at any time.
  static void SetScene(atScene *pScene);

  // Scene Functions

  static int64_t Scene_AddNode(const char *name, const int64_t parentID = atHierarchy_atRootNodeID);
  static void Scene_DeleteNode(int64_t node);
  static int64_t Scene_FindNodeByName(const char *name);
  static atVector<int64_t> Scene_FindNodesByName(const char *name);

  // Node Functions

  static bool Node_SetName(int64_t node, const char *newName);
  static const char* Node_GetName(int64_t node);
  static bool Node_AddComponent(int64_t node, const atCoreComponentType &type, const char *name);
  static atSceneComponent* Node_GetComponent(int64_t node, const int64_t &index);
  static atSceneComponent* Node_GetComponentTyped(int64_t node, const atCoreComponentType &type, const int64_t &index);
  static int64_t Node_GetComponentCount(int64_t node);
  static int64_t Node_GetComponentCountTyped(int64_t node, const atCoreComponentType &type);

  // Component Functions

  // Get transforms
  static atMat4D Transform_GetTransform(atSceneComponent *pComponent);
  static atQuatD Transform_GetOrientation(atSceneComponent *pComponent);
  static atVec3D Transform_GetTranslation(atSceneComponent *pComponent);
  static atVec3D Transform_GetScale(atSceneComponent *pComponent);
  static atVec3D Transform_GetYPR(atSceneComponent *pComponent);
  static atMat4D Transform_GetGlobalTransform(atSceneComponent *pComponent);
  static atVec3D Transform_GetGlobalTranslation(atSceneComponent *pComponent);
  static atVec3D Transform_GetGlobalScale(atSceneComponent *pComponent);
  static atVec3D Transform_GetGlobalYPR(atSceneComponent *pComponent);

  // Set transforms
  static void Transform_SetTranslation(atSceneComponent *pComponent, const atVec3D &translation);
  static void Transform_SetOrientation(atSceneComponent *pComponent, const atQuatD &ori);
  static void Transform_SetYPR(atSceneComponent *pComponent, const atVec3D &ypr);
  static void Transform_SetRotation(atSceneComponent *pComponent, const atMat4D &rotation);
  static void Transform_SetScale(atSceneComponent *pComponent, const atVec3D &scale);

  // Apply transforms
  static void Transform_Translate(atSceneComponent *pComponent, const atVec3D &translation);
  static void Transform_RotateYPR(atSceneComponent *pComponent, const atVec3D &ypr);
  static void Transform_RotateOri(atSceneComponent *pComponent, const atQuatD &ori);
  static void Transform_RotateRot(atSceneComponent *pComponent, const atMat4D &rot);
  static void Transform_Scale(atSceneComponent *pComponent, const atVec3D &scale);

protected:
  static atScene *m_pScene;
};

#endif // atSceneAPI_h__
