
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

#include "atMath.h"
#include "atWindow.h"
#include "atInput.h"
#include "atPrimitiveRenderer.h"
#include "atRenderState.h"

int main(int argc, char **argv)
{
  atUnused(argc, argv);
  atWindow window("atEngine", atWindow::DisplaySize() / 2, true, atWindow::DisplaySize() / 4);
  atPrimitiveRenderer::SetFont("assets/fonts/RomanSerif.ttf");
  atRenderState rs;
  rs.SetBlendEnabled(true);
  while (atInput::Update(false))
  {
    window.Clear(atVec4F(0,0,0,1));
    atPrimitiveRenderer::AddText(0, 0, "Welcome to atEngine");
    atPrimitiveRenderer::Draw(window);
    window.Swap();
  }
  return 0;
}