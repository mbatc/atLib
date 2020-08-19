#include "atEUIButton.h"

atEUIButton::atEUIButton(const atString &label) { SetLabel(label); }

void atEUIButton::UpdateLayout()
{
  atEUIStyle *pStyle = GetStyle();
  atFont *pFont = pStyle->GetFont().HasResource() ? &pStyle->GetFont().GetWriteable().Get() : nullptr;
  atRectF bounds = pFont->CalcTextBounds(m_label);
  SetSize(bounds.Dimensions());
}

bool atEUIButton::Draw(atCanvas2D *pGraphics)
{
  atRectF rect = ScreenRect();
  atEUIStyle *pStyle = GetStyle();
  atFont *pFont = pStyle->GetFont().HasResource() ? &pStyle->GetFont().GetWriteable().Get() : nullptr;
  pGraphics->SetFont(pFont);
  
  atCol bgCol = pStyle->GetColour(atEUIStyle::Color_Background);
  if (IsHovered() && !IsDown())
    bgCol = pStyle->GetColour(atEUIStyle::Color_Highlight);
  pGraphics->AddRectFilled(rect.m_min, rect.m_max, bgCol);
  pGraphics->AddRect(rect.m_min, rect.m_max, pStyle->GetColour(atEUIStyle::Color_Border));
  pGraphics->AddText(rect.m_min, m_label, pStyle->GetColour(atEUIStyle::Color_Text));
  return true;
}

atString atEUIButton::GetLabel() const { return m_label; }
void atEUIButton::SetLabel(const atString &text) { m_label = text; }
