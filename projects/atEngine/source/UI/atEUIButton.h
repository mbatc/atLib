#ifndef atEUIButton_h__
#define atEUIButton_h__

#include "atEUILabel.h"

class atEUIButton : public atEUIComponent
{
public:
  atEUIButton(const atString &label = "");

  virtual void UpdateLayout() override;
  virtual bool Draw(atCanvas2D *pGraphics) override;

  atString GetLabel() const;
  void SetLabel(const atString &text);

protected:
  atString m_label;
};

#endif // atEUIButton_h__
