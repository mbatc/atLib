
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

#ifndef at2DRenderer_h__
#define at2DRenderer_h__

#include "atFilename.h"
#include "atTexture.h"
#include "atWindow.h"
#include "atFont.h"
#include "atRenderable.h"
#include "atRect.h"

class atCanvas2D
{
  struct DrawData
  {
    atVector<atVec2F> uvs;
    atVector<atVec2F> pos;
    atVector<atVec4F> col;
    atVector<uint32_t> indices;
  } m_drawData;

  struct DrawCommand
  {
    atTexture *pTexture = nullptr;
    atRectF clipRect = atRectF::Largest();
    int64_t startElement = 0;
    int64_t elementCount = -1;
  };

public:
  void Draw(const atVec2I &dimensions);
  void Draw(atWindow &wnd);

  void SetFont(atFont *pFont);

  void AddText(const atVec2F &pos, const char *str, atCol color = atColor::white, const float &scale = 1.0f, const char *pEnd = nullptr);
  void AddLine(const atVec2F &a, const atVec2F &b, atCol color, float thinkness = 1.0f);
  void AddRect(const atVec2F &a, const atVec2F &b, atCol color, float rounding = 0.0f, float thickness = 1.0f);
  void AddRectFilled(const atVec2F &a, const atVec2F &b, atCol color, float rounding = 0.0f);
  void AddRectColoured(const atVec2F &a, const atVec2F &b, atCol tlColor, atCol trColor, atCol blColor, atCol brColor, float rounding = 0.0f, float thickness = 1.0f);
  void AddRectColouredFilled(const atVec2F &a, const atVec2F &b, atCol tlColor, atCol trColor, atCol blColor, atCol brColor, float rounding = 0.0f);
  void AddRectTextured(atTexture *pTexture, const atVec2F &a, const atVec2F &b, atCol color = atColor::white, const atVec2F &tlUV = { 0, 0 }, const atVec2F &brUV = { 1, 1 }, float rounding = 0.0f);

  void AddPolyline(atVec2F *pPoints, atCol *pColors, int64_t nPoints, bool closed, float thickness = 1.0f);
  void AddPolylineTextured(atTexture *pTexture, atVec2F *pPoints, atCol *pColors, int64_t nPoints, const atVec2F &minTexCoord = { 0, 0 }, const atVec2F &maxTexCoord = { 1, 1 });
  void AddPolylineConcaveFilled(atVec2F *pPoints, atCol *pColors, int64_t nPoints);

  void AddPath(bool closed, float thickness = 1.0f);
  void AddPathConcaveFilled();
  void PathClear();
  void PathColor(const atCol &color); // Set the colour assigned to new points added
  void PathRect(const atVec2F &a, const atVec2F &b, float rounding = 0.0f, atCol tlCol = atColor::white, atCol trCol = atColor::white, atCol blCol = atColor::white, atCol brCol = atColor::white);
  void PathLineTo(const atVec2F &a);
  void PathArc(const atVec2F &center, float radius, float angleStart, float angle, int64_t nSegments = 10);

  void Clear();
  int64_t VertexCount() const;
  int64_t IndexCount() const;
  int64_t CmdCount() const;

  atRectF clipRect = atRectF::Largest();

protected:
  void Initialise(atGraphics *pAPI);

  const DrawCommand& Command(int64_t index) const;
  const atVector<DrawCommand>& Commands() const;

  DrawCommand& GetDrawCommand(const atRectF &clipRect, atTexture *pTexture);

  atFont* m_pFont = nullptr;
  
  // Drawing paths
  atCol m_pathColVal;
  atVector<atVec2F> m_path;
  atVector<atCol> m_pathCol;
  atVector<DrawCommand> m_cmds;

  atGraphics *m_pGfx = nullptr;

  struct
  {
    atProgram *pProgram = nullptr;
    atGPUBuffer *pUV = nullptr;
    atGPUBuffer *pColour = nullptr;
    atGPUBuffer *pPos = nullptr;
    atGPUBuffer *pIndices = nullptr;
    atSampler *pSampler = nullptr;
    atRenderable renderer;
  } gpu;

};

#endif // at2DRenderer_h__
