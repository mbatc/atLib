
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

// NOTE: This file is used for testing

int main(int argc, char **argv)
{
  atUnused(argc, argv);

  atWindow wnd("My window");
  atCamera cam(wnd, { 0,0, 0.5 });
  atRenderState::SetViewport(atVec4I(0, 0, wnd.GetSize()));
  atRenderable ro;

  ro.SetShader("assets/shaders/color");
  ro.m_textures.Add("diffuseTexture", atHardwareTexture::UploadImage(atString("assets/test/images/brick.jpg")));
  ro.m_positions.Add("POSITION", { {-1, -1, -5 }, { 0, 1, -5 }, { 1, -1, -5 } });
  ro.m_texCoords.Add("TEXCOORD", { { 0, 1 },{ 0.5, 0 },{ 1, 1 } });
  ro.m_colors.Add("COLOR", { { 1,0,0,1 }, { 0,1,0,1 }, { 0,0,1,1 } });
  ro.m_indices = { 0,1,2 };

  DirectX::XMMATRIX dxProj = DirectX::XMMatrixPerspectiveFovRH((float)atDegs2Rads(60.f), 600.f / 800.f, 0.1f, 1000.f);
  DirectX::XMMATRIX dxTrans = DirectX::XMMatrixTranslation(0, 0, 0.5);
  DirectX::XMMATRIX dxView = DirectX::XMMatrixInverse(nullptr, dxTrans);
  DirectX::XMMATRIX dxVP = dxProj * dxView;

  atMat4 atTrans = cam.TransformMat().Transpose();
  atMat4 atView = cam.ViewMat().Transpose();
  atMat4 atProj = cam.ProjectionMat().Transpose();
  atMat4 atVP = atView * atProj;

  float col = 66.f;
  atRenderState::Bind();
  while (atInput::Update())
  {
    atInput::LockMouse(true, wnd.GetSize() / 2);
    wnd.Clear(atVec4F(col / 255.f, col / 255.f, col / 255.f, 1.0f));
    atMat4 VP = cam.ProjectionMat() * cam.ViewMat();
    ro.Draw(VP.Transpose());

    for (const atVec3F &pos : ro.m_positions["POSITION"])
    {
      atVec4F screenPos = VP.Transpose() * atVec4F(pos, 1.0f);
      printf("{ %f, %f, %f, %f }, ", screenPos.x, screenPos.y, screenPos.z, screenPos.w);
    }
    printf("\n");

    wnd.Swap();
    cam.Update(1);

    POINT coord = { wnd.GetWidth() / 2, wnd.GetHeight() / 2 };
  }
  return atWindow_GetResult();
}