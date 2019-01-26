
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#define SOL_CHECK_ARGUMENTS 1
#include "atLua.h"
#include "sol.hpp"

int _ExceptionHandler(lua_State* L, sol::optional<const std::exception&> exception, sol::string_view description)
{
  printf("An exception occurred in a function, here's what it says:\n\t");
  if (exception)
  {
    const std::exception& ex = *exception;
    atString str = ex.what();
    printf("exception: %s\n\n", str.c_str());
  }
  else
  {
    printf("description: %s\n\n", description.data());
  }
  return sol::stack::push(L, description);
}

atLua::atLua()
{
  sol::state lua;
  m_pLua = atNew<sol::state>();
  m_pLua->set_exception_handler(_ExceptionHandler);
  m_pLua->open_libraries();
}

atLua::~atLua() { atDelete(m_pLua); }
sol::state * atLua::GetLua() { return m_pLua; }
bool atLua::RunText(const atString &command) { return m_pLua->script(command.c_str()).valid(); }
bool atLua::RunFile(const atString &filename) { return m_pLua->script_file(filename.c_str()).valid(); }

void atLua::ExposeImGui()
{
  sol::state &lua = *m_pLua;
  lua["atImGui"] = lua.create_table();
  sol::state &gui = lua["atImGui"];
}

void atLua::ExposeScene()
{
  sol::state &lua = *m_pLua;
  if (!lua["atScene"].valid())
    lua["atScene"] = lua.create_table();

  sol::state &gui = lua["atScene"];
}

void atLua::ExposeMathTypes()
{
  sol::state &lua = *m_pLua;
  if (!lua["atMath"].valid())
    lua["atMath"] = lua.create_table();

  sol::state &gui = lua["atMath"];
}

void atLua::ExposeContainers()
{
  sol::state &lua = *m_pLua;
  if (!lua["atScene"].valid())
    lua["atScene"] = lua.create_table();

  sol::state &scene = lua["atScene"];
}

void atLua::ExposeMathFunctions()
{
  sol::state &lua = *m_pLua;
  if (!lua["atMath"].valid())
    lua["atMath"] = lua.create_table();

  sol::state &math = lua["atMath"];
}
