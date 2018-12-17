#include "atSceneRenderer.h"
#include "atRenderState.h"
#include "atCamera.h"

bool atSceneRenderer::Render(atScene *pScene)
{
  atVector<int64_t> cameraIds = pScene->GetActiveCameras();
  atVector<int64_t> ids = pScene->GetNodeIDs();

  for (const int64_t cam : cameraIds)
  {
    atSceneNode *pCameraNode = pScene->GetNode(cam);
    for(atSceneComponent *pComponent : pCameraNode->Components(atSCT_Camera))
    {
      // Render scene for each camera
      atCamera *pCam = (atCamera*)pComponent;
      const atMat4D vp = pCam->ProjectionMat() * pCameraNode->GlobalWorldMat().Inverse();

      atRenderState rs;
      rs.SetViewport(pCam->m_viewport);
      rs.SetDepthRange(pCam->m_depthRange.x, pCam->m_depthRange.y);

      Render(pScene->GetRoot(), vp);
    }
  }
  return true;
}

bool atSceneRenderer::Render(atSceneNode *pNode, const atMat4D &vp)
{
  bool res = pNode->Draw(vp);
  for (atSceneNode *pChild : pNode->Children())
    res &= Render(pChild, vp);
  return res;
}
