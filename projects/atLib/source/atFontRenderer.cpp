#include "atFontRenderer.h"
#include "atFont.h"
#include "atRenderable.h"

static atVector<atFont> s_fonts;
static int64_t s_activeFont = -1;

static atVector<atVec4F> s_colors;
static atVector<atVec3F> s_verts;
static atVector<atVec2F> s_uvs;
static atVector<uint32_t> s_idx;

struct DrawData
{
  int64_t texID;
  atVector<atVec4F> color;
  atVector<atVec3F> verts;
  atVector<atVec2F> uvs;
  atVector<uint32_t> indices;
};

static atVector<DrawData> s_drawList;

DrawData &GetDrawData(int64_t texID)
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
  s_fonts.push_back(atFont(font, 32));
  s_activeFont = s_fonts.size() - 1;
}

void atFontRenderer::Bake(const int64_t x, const int64_t y, const atString &text)
{
  int64_t xPos = 0;
  for (const char c : text)
  {
    atFont &font = s_fonts[s_activeFont];
    atFont::Glyph g = font.GetGlyph(c);
    DrawData &dd = GetDrawData(font.GetTextureID());
    uint32_t i = (uint32_t)s_verts.size();
    dd.verts.push_back(atVec3F{ (float)xPos,            (float)y, 0 });
    dd.verts.push_back(atVec3F{ (float)xPos + g.width,  (float)y, 0 });
    dd.verts.push_back(atVec3F{ (float)xPos + g.width,  (float)y + g.height, 0 });
    dd.verts.push_back(atVec3F{ (float)xPos,            (float)y + g.height, 0 });
    dd.uvs.push_back(atVec2F{ g.tl.x, g.tl.y });
    dd.uvs.push_back(atVec2F{ g.br.x, g.tl.y });
    dd.uvs.push_back(atVec2F{ g.br.x, g.br.y });
    dd.uvs.push_back(atVec2F{ g.tl.x, g.br.y });
    dd.color.push_back(atVec4F::one());
    dd.color.push_back(atVec4F::one());
    dd.color.push_back(atVec4F::one());
    dd.color.push_back(atVec4F::one());
    dd.indices.push_back({ i, i + 1, i + 2, i, i + 2, i + 3 });
    xPos += g.advance * g.scale;
  }
}

void atFontRenderer::Draw(const atWindow &wnd)
{
  atRenderable ro;
  for (DrawData &dd : s_drawList)
  {
    ro.SetShader("assets/shaders/text");
    ro.SetChannel("samplerType", AT_INVALID_ID, atRRT_Sampler);
    ro.SetChannel("font0", dd.texID, atRRT_Texture);
    ro.SetChannel("POSITION", dd.verts, atRRT_VertexData);
    ro.SetChannel("COLOR", dd.color, atRRT_VertexData);
    ro.SetChannel("TEXCOORD", dd.uvs, atRRT_VertexData);
    ro.SetChannel("idxBuffer", dd.indices, atRRT_Indices);
    ro.SetChannel("mvp", atMat4(atMatrixOrtho(wnd.Width(), wnd.Height(), -1, 1)), atRRT_Variable);
    ro.Draw();

    dd.verts.clear();
    dd.color.clear();
    dd.uvs.clear();
    dd.indices.clear();
  }
}
