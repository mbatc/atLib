#include "atEngineUI.h"
#include "atEUIFrame.h"
#include "atEUILabel.h"
#include "atEApp.h"
#include "atEUIButton.h"
#include "atRenderState.h"

atEngineUI::atEngineUI()
{
  m_pRoot = atNew<atEUIFrame>();

  atEUILabel *pLabel = m_pRoot->AddChild<atEUILabel>("Label 1");
  pLabel->SetText("This is some text");

  atEUILabel *pLabel2 = m_pRoot->AddChild<atEUILabel>("Label 2");
  pLabel2->SetText("This is some more text");
  pLabel2->SetPosition({ 0, 24 });

  atEUIButton *pButton = m_pRoot->AddChild<atEUIButton>("Button 1", " A Button");
  pButton->SetPosition({ 0, 48 });
}

void atEngineUI::Update()
{
  atEApp *pApp = atEApp::Instance();

  // Fit the root component to the window
  m_pRoot->SetPosition({ 0, 0 });
  m_pRoot->SetSize(pApp->Window()->Size());

  // Update the UI layout
  m_pRoot->UpdateLayout();

  // Process inputs
  m_pRoot->ProcessInput();
}

bool atEngineUI::Draw()
{
  // Draw the UI to the canvas
  if (m_pRoot->Draw(&m_canvas))
    m_pRoot->DrawChildren(&m_canvas);

  // Draw the canvas to the window
  atEApp *pApp = atEApp::Instance();

  atRenderState rs;
  rs.SetBlendEnabled(true);
  rs.SetDepthReadEnabled(false);
  rs.SetDepthWriteEnabled(false);

  m_canvas.Draw(*pApp->Window());
  m_foreground.Draw(*pApp->Window());
  m_background.Draw(*pApp->Window());
  return true;
}

atCanvas2D& atEngineUI::UICanvas() { return m_canvas; }
atCanvas2D& atEngineUI::Foreground() { return m_foreground; }
atCanvas2D& atEngineUI::Background() { return m_background; }
