#ifndef atLua_h__
#define atLua_h__

#include "atString.h"

namespace sol { class state; };

class atLua
{
public:
  atLua();
  ~atLua();

  bool RunText(const atString &command);
  bool RunFile(const atString &filename);

  sol::state* GetLua();
protected:
  sol::state *m_pLua;
};

#endif // atLua_h__
