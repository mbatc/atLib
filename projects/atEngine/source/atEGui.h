#ifndef atEGui_h__
#define atEGui_h__

#include "../3rdParty/CTML/include/ctml.h"

#include "atWindow.h"

class atEGui
{
public:
  atEGui();
  ~atEGui();

  void Update(atWindow *pWindow);
  void Draw();

protected:
  void UpdateIO();

  ctml::ctmlDocument m_document;
};

#endif // atEGui_h__
