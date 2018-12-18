
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

    // Retrieve each of the nodes camera components
    for(atSceneComponent *pComponent : pCameraNode->Components(atSCT_Camera))
    {
      atCamera *pCam = (atCamera*)pComponent;
      
      // Setup viewport
      atRenderState rs;
      atVec4I viewport = pScene->m_viewport;
      if ((pCam->Viewport().x >= 0 && pCam->Viewport().y >= 0 && pCam->Viewport().z >= 0 && pCam->Viewport().w >= 0))
        viewport = atVec4I(viewport.xy(), 0, 0) + pCam->Viewport();
      else
        pCam->m_aspect = double(viewport.z) / double(viewport.w);
      
      rs.SetViewport(viewport);
      rs.SetDepthRange(pCam->m_depthRange.x, pCam->m_depthRange.y);
      
      // Render scene
      Render(pScene->GetRoot(), pCam->ProjectionMat() * pCameraNode->GlobalWorldMat().Inverse());
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
