#include "atFontRenderer.h"
#include "atFont.h"
#include "atRenderable.h"
#include "atInput.h"

static atVector<atFont> s_fonts;
static int64_t s_activeFont = -1;

struct DrawData
{
  int64_t texID;
  atVector<atVec4F> color;
  atVector<atVec3F> verts;
  atVector<atVec2F> uvs;
  atVector<uint32_t> indices;
};

static atVector<DrawData> s_drawList;

DrawData& GetDrawData(int64_t texID)
{
  for (DrawData &dd : s_drawList)
    if (dd.texID == texID)
      return dd;
  s_drawList.push_back(DrawData());
  s_drawList.back().texID = texID;
  return s_drawList.back();
}

void atFontRenderer::SetFont(const atFilename &font)
{
  for (const atFont &f : s_fonts)
    if (f.Filename() == font)
    {
      s_activeFont = &f - s_fonts.begin();
      return;
    }
  s_fonts.push_back(atFont(font, 24));
  s_activeFont = s_fonts.size() - 1;
}

void atFontRenderer::Bake(const int64_t x, const int64_t y, const atString &text)
{
  static float z = 0.0f;
  if (atInput::KeyDown(atKC_Up)) z += 0.01f;
  if (atInput::KeyDown(atKC_Down)) z -= 0.01f;
  atFont &font = s_fonts[s_activeFont];
  DrawData &dd = GetDrawData(font.GetTextureID(false));
  int64_t xPos = x;
  int64_t yPos = -y - font.Height();
  int64_t rowHeight = 0;
  for (const char c : text)
  {
    atFont::Glyph g = font.GetGlyph(c);
    uint32_t i = (uint32_t)dd.verts.size();
    dd.verts.push_back(atVec3F{ (float)xPos,            (float)yPos - g.yOff, z });
    dd.verts.push_back(atVec3F{ (float)xPos + g.width,  (float)yPos - g.yOff, z });
    dd.verts.push_back(atVec3F{ (float)xPos + g.width,  (float)yPos - g.height - g.yOff, z });
    dd.verts.push_back(atVec3F{ (float)xPos,            (float)yPos - g.height - g.yOff, z });
    dd.uvs.push_back(atVec2F{ g.tl.x, g.br.y });
    dd.uvs.push_back(atVec2F{ g.br.x, g.br.y });
    dd.uvs.push_back(atVec2F{ g.br.x, g.tl.y });
    dd.uvs.push_back(atVec2F{ g.tl.x, g.tl.y });
    dd.color.push_back(atVec4F::one());
    dd.color.push_back(atVec4F::one());
    dd.color.push_back(atVec4F::one());
    dd.color.push_back(atVec4F::one());
    dd.indices.push_back({ i, i + 1, i + 2, i, i + 2, i + 3 });
    xPos += g.advance;
    rowHeight = atMin(rowHeight, yPos - g.height);
    if (c == '\n')
    {
      yPos = rowHeight;
      xPos = x;
    }
  }
}

void atFontRenderer::Draw(const atWindow &wnd)
{
  atRenderable ro;
  for (atFont &f : s_fonts)
    f.GetTextureID();

  for (DrawData &dd : s_drawList)
  {
    ro.SetShader("assets/shaders/text");
    ro.SetChannel("samplerType", AT_INVALID_ID, atRRT_Sampler);
    ro.SetChannel("font0", dd.texID, atRRT_Texture);
    ro.SetChannel("POSITION", dd.verts, atRRT_VertexData);
    ro.SetChannel("COLOR", dd.color, atRRT_VertexData);
    ro.SetChannel("TEXCOORD", dd.uvs, atRRT_VertexData);
    ro.SetChannel("idxBuffer", dd.indices, atRRT_Indices);
    ro.SetChannel("mvp", atMat4(atMatrixOrtho((float)wnd.Width(), (float)wnd.Height(), -1.f, 1.f)), atRRT_Variable);
    ro.Draw();

    dd.verts.clear();
    dd.color.clear();
    dd.uvs.clear();
    dd.indices.clear();
  }
}

atVec2I atFontRenderer::TextSize(const atString &text)
{
  atVec2I64 pos = 0;
  atVec2I64 size = 0;
  atFont &font = s_fonts[s_activeFont];
  DrawData &dd = GetDrawData(font.GetTextureID(false));

  for (const char c : text)
  {
    atFont::Glyph g = font.GetGlyph(c);
    pos.x += g.advance;
    size.x = atMax(size.x , pos.x + g.advance);
    size.y = atMax(size.y, pos.y + g.height);
    if (c == '\n')
    {
      pos.y = size.y;
      pos.x = 0;
    }
  }
  return atVec2I();
}

atVec4I atFontRenderer::TextRect(const int64_t x, const int64_t y, const atString & text)
{
  return atVec4I();
}
