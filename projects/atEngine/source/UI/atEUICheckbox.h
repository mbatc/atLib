#ifndef atEUICheckbox_h__
#define atEUICheckbox_h__

#include "atEUIComponent.h"

class atEUICheckbox : public atEUIComponent
{
public:
  void OnToggle(std::function<void(atEUICheckbox *pCheckbox)> newState);

protected:
  bool m_state;
};

#endif // atEUICheckbox_h__
