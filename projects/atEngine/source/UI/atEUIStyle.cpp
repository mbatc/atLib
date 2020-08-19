#include "atEUIStyle.h"
#include "atEUIStyle.h"
#include "atEResources.h"

static atString _colourNames[atEUIStyle::Color_Count] =
{
  "Background",
  "Text",
  "TextAccent",
  "Border",
  "Highlight",
};

atEUIStyle::atEUIStyle()
{
  SetFont(TS_Regular, "assets/fonts/RomanSerif.ttf");
  SetFont(TS_Bold, "assets/fonts/RomanSerif.ttf");
  SetFont(TS_Italics, "assets/fonts/RomanSerif.ttf");

  if (false)
  {
    m_colours[Color_Background] = atColor::Pack(244, 241, 222);
    m_colours[Color_Text] = atColor::Pack(61, 64, 91);
    m_colours[Color_TextAccent] = atColor::Pack(224, 112, 95);
    m_colours[Color_Border] = atColor::Pack(242, 204, 143);
    m_colours[Color_Highlight] = atColor::Pack(129, 178, 154, 50);
  }

  if (true)
  {
    m_colours[Color_Background] = atColor::Pack(59, 58, 65);
    m_colours[Color_Text] = atColor::Pack(215, 217, 208);
    m_colours[Color_TextAccent] = atColor::Pack(206, 186, 207);
    m_colours[Color_Border] = atColor::Pack(4, 4, 4);
    m_colours[Color_Highlight] = atColor::Pack(17, 157, 164);
  }
}

bool _ValidColour(const atEUIStyle::Color &colID) { return colID >= 0 && colID < atEUIStyle::Color_Count; }

atEUIStyle* atEUIStyle::GlobalStyle()
{
  static atEUIStyle style;
  return &style;
}

atEUIStyle::Color atEUIStyle::GetColourID(const atString &name)
{
  for (int64_t i = 0; i < Color_Count; ++i)
    if (_colourNames[i].compare(name, atSCO_None))
      return atEUIStyle::Color(i);
  return Color_None;
}

atString atEUIStyle::GetColourName(const Color &colID) { return _ValidColour(colID) ? _colourNames[colID] : ""; }

void atEUIStyle::SetColour(const Color &colID, const atCol &color) { if (_ValidColour(colID)) m_colours[colID] = color; }
void atEUIStyle::SetColour(const Color &colID, const atVec4D &color) { if (_ValidColour(colID)) m_colours[colID] = atColor::Pack(color); }
atCol atEUIStyle::GetColour(const Color &colID) const { return _ValidColour(colID) ? m_colours[colID] : 0; }
atVec4D atEUIStyle::GetColour4(const Color &colID) const { return atColor::UnPack<double>(GetColour(colID)); }

void atEUIStyle::SetFont(const TextStyle &style, const atFilename &path, const int64_t &scale, const int64_t &resolution)
{
  FontInfo &info = m_fonts[style];
  atObjectDescriptor request;
  request.Add("url") = path;
  request.Add("scale") = scale;
  request.Add("resolution") = resolution;
  info.path = path;
  info.scale = scale;
  info.resolution = resolution;
  info.font = atEResources::Instance()->GetManager().Request<atFont>(request);
}

atResourceHandleT<atFont> atEUIStyle::GetFont(const TextStyle &style) { return m_fonts[style].font; }

bool atSerialize(atObjectDescriptor *pSerialized, const atEUIStyle &src)
{
  for (int64_t i = 0; i < atEUIStyle::Color_Count; ++i)
  {
    atEUIStyle::Color colID = (atEUIStyle::Color)i;
    pSerialized->Add(atEUIStyle::GetColourName(colID)).Serialize(src.GetColour(colID));
  }
  return true;
}

bool atDeserialize(const atObjectDescriptor &serialized, atEUIStyle *pDst)
{
  for (int64_t i = 0; i < atEUIStyle::Color_Count; ++i)
  {
    atEUIStyle::Color colID = (atEUIStyle::Color)i;
    atCol colour = pDst->GetColour(colID);
    serialized.Get(atEUIStyle::GetColourName(colID)).Deserialize(&colour);
    pDst->SetColour(colID, colour);
  }
  return true;
}