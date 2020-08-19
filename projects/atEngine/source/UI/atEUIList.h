#ifndef atEUILabel_h__
#define atEUILabel_h__

#include "atEUIComponent.h"

class atEUIList : public atEUIComponent
{
public:
  atEUIList(const atString &label);

  virtual bool Draw(atCanvas2D *pGraphics) override;
  virtual bool DrawChildren(atCanvas2D *pGraphics) override;
};

#endif // atEUILabel_h__
