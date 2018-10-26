
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

#include "atInput.h"
#include "atRenderState.h"
#include "atCamera.h"
#include "atGraphicsModel.h"
#include "atFile.h"
#include "atMeshParser.h"

// NOTE: This file is used for testing

int main(int argc, char **argv)
{
  atUnused(argc, argv);
 
  atMesh mesh;
  atMeshReader::Read(atFilename("assets/test/models/level.obj"), &mesh);

  mesh.MakeValid();
  mesh.FlipTextures(false, true);
  mesh.DiscoverTextures();

  atFile file;
  file.Open(atFilename("level.atm"), atFM_Write | atFM_Binary);
  atStreamWrite(&file, &mesh, 1);
  file.Close();
  mesh = atMesh();
  file.Open(atFilename("level.atm"), atFM_Read | atFM_Binary);
  atStreamRead(&file, &mesh, 1);

  atGraphicsModel model(mesh);
  atWindow wnd("My window", { 1800, 980 });
  atCamera cam(wnd, { 0,0, 5 });
  atRenderState::SetViewport(atVec4I(0, 0, wnd.GetSize()));
  
  float col = 125.f;
  atRenderState::Bind();
  while (atInput::Update())
  {
    atInput::LockMouse(true, wnd.GetSize() / 2);
    wnd.Clear(atVec4F(col / 255.f, col / 255.f, col / 255.f, 1.0f));
    model.Draw(cam.ProjectionMat() * cam.ViewMat());
    wnd.Swap();
    cam.Update(1);
  }
  return atWindow_GetResult();
}