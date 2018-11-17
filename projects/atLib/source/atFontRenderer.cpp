#include "atFontRenderer.h"
#include "atFont.h"
#include "atRenderable.h"

static atVector<atFont> s_fonts;
static int64_t s_activeFont = -1;

static atVector<atVec4F> s_colors;
static atVector<atVec3F> s_verts;
static atVector<atVec2F> s_uvs;
static atVector<uint32_t> s_idx;

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
  int64_t xPos = x;
  for (const char c : text)
  {
    atFont::Glyph g = s_fonts[s_activeFont].GetGlyph(c);
    uint32_t i = (uint32_t)s_verts.size();
    s_verts.push_back(atVec3F{ (float)xPos,            (float)y, 0 });
    s_verts.push_back(atVec3F{ (float)xPos + g.width,  (float)y, 0 });
    s_verts.push_back(atVec3F{ (float)xPos + g.width,  (float)y + g.height, 0 });
    s_verts.push_back(atVec3F{ (float)xPos,            (float)y + g.height, 0 });
    s_uvs.push_back(atVec2F{ g.tl.x, g.tl.y });
    s_uvs.push_back(atVec2F{ g.br.x, g.tl.y });
    s_uvs.push_back(atVec2F{ g.br.x, g.br.y });
    s_uvs.push_back(atVec2F{ g.tl.x, g.br.y });
    s_colors.push_back(atVec4F::one());
    s_colors.push_back(atVec4F::one());
    s_colors.push_back(atVec4F::one());
    s_colors.push_back(atVec4F::one());
    s_idx.push_back({ i, i + 1, i + 2, i, i + 2, i + 3 });
    xPos += g.advance;
  }
}

void atFontRenderer::Draw(const atWindow &wnd)
{
  atRenderable ro;
  ro.SetShader("assets/shaders/text");
  ro.SetChannel("POSITION", s_verts, atRRT_VertexData);
  ro.SetChannel("COLOR", s_colors, atRRT_VertexData);
  ro.SetChannel("TEXCOORD", s_uvs, atRRT_VertexData);
  ro.SetChannel("idxBuffer", s_uvs, atRRT_Indices);
  ro.SetChannel("mvp", atMat4(atMatrixOrtho(wnd.Width(), wnd.Height(), -1, 1)), atRRT_Variable);
  ro.Draw();
}
