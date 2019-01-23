#include "atLua.h"
#include "sol.hpp"

static bool _ErrorReport(const sol::protected_function_result &result)
{
  if (!result.valid())
  {
    sol::error err = result;
    atString str = err.what();
    printf("%s\n\n", str.c_str());
    return false;
  }
  return true;
}

atLua::atLua()
{
  sol::state lua;
  m_pLua = atNew<sol::state>();
  m_pLua->open_libraries();
}

atLua::~atLua() { atDelete(m_pLua); }
sol::state * atLua::GetLua() { return m_pLua; }
bool atLua::RunText(const atString &command) { return _ErrorReport(m_pLua->script(command.c_str())); }
bool atLua::RunFile(const atString &filename) { return _ErrorReport(m_pLua->script_file(filename.c_str())); }
