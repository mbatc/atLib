#ifndef atEUILabel_h__
#define atEUILabel_h__

#include "atEUIComponent.h"

class atEUILabel : public atEUIComponent
{
public:
  atEUILabel(const atString &label = "");

  virtual void UpdateLayout() override;
  virtual bool Draw(atCanvas2D *pGraphics) override;

  // Set the label text
  void SetText(const atString &label);

  // Get the label text
  const atString& GetText() const;

protected:
  atString m_text;
};

#endif // atEUILabel_h__
