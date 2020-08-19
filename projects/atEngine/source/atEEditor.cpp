#include "atEEditor.h"

static atEEditor *_pInstance = nullptr;

atEEditor::atEEditor() {}

atEEditor* atEEditor::Instance()
{
  if (!_pInstance)
    _pInstance = new atEEditor;
  return _pInstance;
}

void atEEditor::Destroy()
{
  if (_pInstance)
    delete _pInstance;
}

void atEEditor::Update()
{
  m_ui.Update();
}

void atEEditor::Render()
{
  m_ui.Draw();
}
