#include "atEGui.h"
#include "atRenderable.h"
#include "atRenderState.h"
#include "atDXBuffer.h"
#include "atDXTexture.h"

static const char* _vsSource =
R"(

cbuffer vertexBuffer : register(b0)
{
  float4x4 mvp;
};

struct VS_INPUT
{
  float2 pos : POSITION;
  float4 col : COLOR0;
  float2 uv  : TEXCOORD0;
};

struct PS_INPUT
{
  float4 pos : SV_POSITION;
  float4 col : COLOR0;
  float2 uv  : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input)
{
  PS_INPUT output;
  output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));
  output.col = input.col;
  output.uv  = input.uv;
  return output;
}

)";

static const char* _psSource =
R"(

struct PS_INPUT
{
  float4 pos : SV_POSITION;
  float4 col : COLOR0;
  float2 uv  : TEXCOORD0;
};

sampler sampler0;
Texture2D texture0;

float4 main(PS_INPUT input) : SV_Target
{
  float4 out_col = input.col * texture0.Sample(sampler0, input.uv);
  return out_col;
}

)";

// This function will be called when the font texture is created.
static bool _CreateFontTex(ctml::ctmlTextureID *pTexID, ctml::ctmlColor *pData, const ctml::Vec2 &size)
{
  std::shared_ptr<atDXTexture> *pTexture = atNew<std::shared_ptr<atDXTexture>>(std::make_shared<atDXTexture>(atImage(pData, {size.x, size.y})));
  *pTexID = (ctml::ctmlTextureID*)pTexture;
  return true;
}

static bool _ReleaseFontTex(ctml::ctmlTextureID *pTexID)
{
  std::shared_ptr<atDXTexture> *pTexture = (std::shared_ptr<atDXTexture>*)*pTexID;
  if (!pTexture)
    return false;

  atDelete(pTexture);
  return true;
}


atEGui::atEGui()
{
  ctml::Initialise();

  ctml::GetContext().fonts.push_back(ctml::ctmlFont("RomanSerif.ttf"));
  ctml::GetContext().fontsConfig.activeFont = 0;

  ctml::GetContext().fontsConfig.releaseFontTexFunc = _ReleaseFontTex;
  ctml::GetContext().fontsConfig.createFontTexFunc = _CreateFontTex;

  m_document.LoadFile("index.html");
}

atEGui::~atEGui()
{
}

void atEGui::Update(atWindow *pWindow)
{
  UpdateIO();

  ctml::GetCanvas().m_displayPos = { 0, 0 };
  ctml::GetCanvas().m_displaySize = { (float)pWindow->Size().x, (float)pWindow->Size().y };
  ctml::BeginFrame();
  ctml::UpdateDocument(&m_document);
}

void atEGui::Draw()
{
  ctml::DrawDocument(m_document);

  ctml::ctmlCanvas &canvas = ctml::GetCanvas();

  static atRenderable ro;
  static atVector<atVec2F> position;
  static atVector<atVec2F> uvs;
  static atVector<uint32_t> colours;
  static atVector<uint32_t> indices;

  atRenderState rs;

  // Create and grow vertex/index buffers if needed
  int64_t vtxCount = canvas.VertexCount();
  int64_t idxCount = canvas.IndexCount();
  indices.resize(idxCount);
  position.resize(vtxCount);
  colours.resize(vtxCount);
  uvs.resize(vtxCount);

  int64_t vtxOffset = 0;
  int64_t idxOffset = 0;
  int64_t posOffset = (int64_t)&((ctml::ctmlVertex*)0)->pos;
  int64_t uvOffset = (int64_t)&((ctml::ctmlVertex*)0)->uv;
  int64_t colOffset = (int64_t)&((ctml::ctmlVertex*)0)->col;
  for (int64_t cmdIdx = 0; cmdIdx < canvas.CmdCount(); ++cmdIdx)
  {
    const ctml::ctmlDrawCommand& cmd = canvas.Command(cmdIdx);

    for (int64_t v = 0; v < (int64_t)cmd.vtx.size(); ++v)
    {
      memcpy(&position[vtxOffset + v], (uint8_t*)(cmd.vtx.data() + v) + posOffset, sizeof(ctml::ctmlVertex::pos));
      memcpy(&colours[vtxOffset + v], (uint8_t*)(cmd.vtx.data() + v) + colOffset, sizeof(ctml::ctmlVertex::col));
      memcpy(&uvs[vtxOffset + v], (uint8_t*)(cmd.vtx.data() + v) + uvOffset, sizeof(ctml::ctmlVertex::uv));
    }

    memcpy(indices.data() + idxOffset, cmd.idx.data(), cmd.idx.size() * sizeof(ctml::ctmlIndex));
    vtxOffset += cmd.vtx.size();
    idxOffset += cmd.idx.size();
  }

  // Setup orthographic projection matrix into our constant buffer
  // Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right).
  {
    float L = canvas.m_displayPos.x;
    float R = canvas.m_displayPos.x + canvas.m_displaySize.x;
    float T = canvas.m_displayPos.y;
    float B = canvas.m_displayPos.y + canvas.m_displaySize.y;
    atMat4F mvp = atMatrixOrtho(L, R, T, B, 0.0f, 1.0f);

    // atMat4F mvp =
    // {
    //   2.0f / (R - L), 0.0f, 0.0f, 0.0f,
    //   0.0f, 2.0f / (T - B), 0.0f, 0.0f,
    //   0.0f,         0.0f,           0.5f,       0.0f,
    //   (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f,

    ro.SetUniform("mvp", mvp);
  }

  std::shared_ptr<atDXBuffer> idxBuf = std::make_shared<atDXBuffer>(indices, atBT_IndexData);
  std::shared_ptr<atDXBuffer> positionBuf = std::make_shared<atDXBuffer>(position, atBT_VertexData);
  std::shared_ptr<atDXBuffer> uvBuf = std::make_shared<atDXBuffer>(uvs, atBT_VertexData);
  std::shared_ptr<atDXBuffer> colBuf = std::make_shared<atDXBuffer>(colours, atBT_VertexData);

  // Setup viewport
  rs.SetViewport(atVec4D(0.0f, 0.0f, canvas.m_displayPos.x, canvas.m_displayPos.y));

  // Setup render state
  rs.SetBlendEnabled(true);
  rs.SetScissorEnabled(true);
  rs.SetDepthReadEnabled(false);
  rs.SetDepthWriteEnabled(false);

  // Render command lists
  vtxOffset = 0;
  idxOffset = 0;
  atVec2F clip_off = { canvas.m_displayPos.x, canvas.m_displayPos.y };
  for (int64_t cmdIdx = 0; cmdIdx < canvas.CmdCount(); ++cmdIdx)
  {
    const ctml::ctmlDrawCommand &cmd = canvas.Command(cmdIdx);

    // Apply scissor/clipping rectangle
    rs.SetScissor({
      cmd.clipRect.x - clip_off.x,
      cmd.clipRect.y - clip_off.y,
      cmd.clipRect.z - clip_off.x,
      cmd.clipRect.w - clip_off.y });

    // Bind texture, Draw
    std::shared_ptr<atDXTexture> *pTexture = (std::shared_ptr<atDXTexture>*)cmd.texID;
    ro.SetTexture("texture0", *pTexture);
    ro.SetAttribute("COLOR0", colBuf);
    ro.SetAttribute("POSITION", positionBuf);
    ro.SetAttribute("TEXCOORD0", uvBuf);
    ro.Draw(true, atGFX_PT_TriangleList, cmd.idx.size(), idxOffset, vtxOffset);
    idxOffset += (int64_t)cmd.idx.size();
    vtxOffset += (int64_t)cmd.vtx.size();
  }
}

void atEGui::UpdateIO()
{
}
