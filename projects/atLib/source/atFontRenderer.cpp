#include "atFontRenderer.h"
#include "atFont.h"
#include "atRenderable.h"
#include "atInput.h"
#include "atIntersects.h"
#include "atRenderState.h"

static atVector<atFont> s_fonts;
static atVector<atVec4F> s_colStack;
static atVector<atVec4I> s_clipStack;

static int64_t s_activeFont = -1;

struct DrawData
{
  int64_t texID;
  atHashMap<atVec4I64, atVector<atVec4F>> color;
  atHashMap<atVec4I64, atVector<atVec3F>> verts;
  atHashMap<atVec4I64, atVector<atVec2F>> uvs;
  atHashMap<atVec4I64, atVector<uint32_t>> indices;
};

static atVector<DrawData> s_drawList;


static void _AdvanceCursor(const char c, const atFont::Glyph &g, int64_t *pLineHeight, atVec2I *pPos, const atVec2I &tl = { 0, 0 })
{
  pPos->x += (int32_t)g.advance;
  *pLineHeight = atMax(*pLineHeight, pPos->y + g.height);
  if (c == '\n')
  {
    pPos->y = (int32_t)*pLineHeight;
    pPos->x = tl.x;
    *pLineHeight = 0;
  }
}

static atAABB<int32_t> _TextBounds(const atString &text)
{
  atVec2I pos = 0;
  int64_t lineHeight = 0;
  atFont &font = s_fonts[s_activeFont];
  atAABB<int32_t> bounds;
  for (const char c : text)
  {
    atFont::Glyph g = font.GetGlyph(c);
    atVec4I rect = { pos.x,  pos.y + g.yOff, pos.x + (int32_t)g.width,  pos.y + (int32_t)g.height + g.yOff };
    bounds.GrowToContain(atAABB<int32_t>({ rect.x, rect.y, 0 },{ rect.z, rect.w, 0 }));
    _AdvanceCursor(c, g, &lineHeight, &pos);
  }
  return bounds;
}

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

void atFontRenderer::Bake(const int64_t x, const int64_t y, const atString &text, const atVec2F &pivot)
{
  atUnused(pivot);
  atFont &font = s_fonts[s_activeFont];
  DrawData &dd = GetDrawData(font.GetTextureID(false));

  atVec4I clip = s_clipStack.size() ? s_clipStack.back() : atVec4I64(-INT32_MAX, -INT32_MAX, INT32_MAX, INT32_MAX);
  atVec4F color = s_colStack.size() ? s_colStack.back() : atVec4F::one();

  atAABB<int32_t> bounds = _TextBounds(text);
  atVec2I offset = { atMin(bounds.m_min.x, 0), atMin(bounds.m_min.y, 0) };
  atVec2I tl = atVec2I{ x, y } - pivot * bounds.Dimensions().xy();
  
  if (!atIntersects(clip, atVec4I(tl, tl + bounds.Dimensions().xy())))
    return;

  tl -= offset;
  atVec2I cursor = tl;
  int64_t rowHeight = 0;

  dd.indices.TryAdd(clip);
  dd.verts.TryAdd(clip);
  dd.color.TryAdd(clip);
  dd.uvs.TryAdd(clip);
  atVector<uint32_t> *pIdx = dd.indices.TryGet(clip);
  atVector<atVec3F> *pVert = dd.verts.TryGet(clip);
  atVector<atVec4F> *pCol = dd.color.TryGet(clip);
  atVector<atVec2F> *pUV = dd.uvs.TryGet(clip);

  for (const char c : text)
  {
    atFont::Glyph g = font.GetGlyph(c);
    uint32_t i = (uint32_t)pVert->size();
    atVec4F rect = { 
      (float)cursor.x + g.xOff,  
      (float)cursor.y + g.yOff, 
      (float)cursor.x + g.xOff + g.width,  
      (float)cursor.y + g.height + g.yOff 
    };

    _AdvanceCursor(c, g, &rowHeight, &cursor, tl);
    if (!atIntersects(clip, atVec4I(rect)))
      continue;

    pVert->push_back(atVec3F{ rect.x, rect.y, 0 });
    pVert->push_back(atVec3F{ rect.z, rect.y, 0 });
    pVert->push_back(atVec3F{ rect.z, rect.w, 0 });
    pVert->push_back(atVec3F{ rect.x, rect.w, 0 });
    pUV->push_back(atVec2F{ g.tl.x, g.br.y });
    pUV->push_back(atVec2F{ g.br.x, g.br.y });
    pUV->push_back(atVec2F{ g.br.x, g.tl.y });
    pUV->push_back(atVec2F{ g.tl.x, g.tl.y });
    pCol->push_back(color);
    pCol->push_back(color);
    pCol->push_back(color);
    pCol->push_back(color);
    pIdx->push_back({ i, i + 1, i + 2, i, i + 2, i + 3 });
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
    ro.SetChannel("mvp", atMat4(atMatrixOrtho((float)wnd.Width(), (float)wnd.Height(), -1.f, 1.f)), atRRT_Variable);

    for (const atVec4I64 key : dd.verts.GetKeys())
    {
      atRenderState::SetScissor(key);
      ro.SetChannel("POSITION", dd.verts[key], atRRT_VertexData);
      ro.SetChannel("COLOR", dd.color[key], atRRT_VertexData);
      ro.SetChannel("TEXCOORD", dd.uvs[key], atRRT_VertexData);
      ro.SetChannel("idxBuffer", dd.indices[key], atRRT_Indices);
      ro.Draw();
    }

    dd.verts.Clear();
    dd.color.Clear();
    dd.uvs.Clear();
    dd.indices.Clear();
  }
}

atVec4I atFontRenderer::TextRect(const int64_t x, const int64_t y, const atString &text, const atVec2F &pivot)
{
  atAABB<int32_t> bounds = _TextBounds(text);
  atVec2I offset = { atMin(bounds.m_min.x, 0), atMin(bounds.m_max.y, 0) };
  return atVec4I(bounds.m_min.xy() + offset + atVec2I{ x, y }, bounds.m_max.xy() + offset + atVec2I{ x, y });
}

atVec2I atFontRenderer::TextSize(const atString &text) { return _TextBounds(text).Dimensions().xy(); }
void atFontRenderer::PushColour(const atVec4F &color) { s_colStack.push_back(color); }
void atFontRenderer::PushClipRect(const atVec4I &rect) { s_clipStack.push_back(rect); }
void atFontRenderer::PopColour(const int64_t count) { s_colStack.erase(atMax(0, s_colStack.size() - count), (atMin(count, s_colStack.size()))); }
void atFontRenderer::PopClipRect(const int64_t count) { s_clipStack.erase(atMax(0, s_clipStack.size() - count), (atMin(count, s_clipStack.size()))); }
