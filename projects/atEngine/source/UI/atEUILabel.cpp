#include "atEUILabel.h"
#include "atCanvas2D.h"

atEUILabel::atEUILabel(const atString &label) { SetText(label); }

void atEUILabel::UpdateLayout()
{
}

bool atEUILabel::Draw(atCanvas2D *pGraphics)
{
  atEUIStyle *pStyle = GetStyle();
  atRectF rect = ScreenRect();

  pGraphics->AddText(rect.m_min, m_text, pStyle->GetColour(atEUIStyle::Color_Text));
  // pGraphics->PopColour();
  // pGraphics->PopClipRect();
  return true;
}

void atEUILabel::SetText(const atString &label) { m_text = label; }
const atString& atEUILabel::GetText() const { return m_text; }
