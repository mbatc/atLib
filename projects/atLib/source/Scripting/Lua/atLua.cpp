
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
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

#include "atLua.h"
#include "sol/sol.hpp"
#include "atImGui.h"
#include "atInput.h"
#include "atCamera.h"

inline void _LuaPanic(sol::optional<std::string> maybe_msg) 
{
  printf("Lua is in a panic state and will now abort() the application\n");
  if (maybe_msg)
    printf("\terror message: %s\n", maybe_msg.value().c_str());
}

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

bool _LuaCall(const sol::state &s, const sol::protected_function_result &res)
{
  if (!res.valid())
  {
    atString err = "sol: ";
    err += to_string(res.status()).c_str();
    err += " error";
    std::exception_ptr eptr = std::current_exception();
    if (eptr) {
      err += " with a ";
      try {
        std::rethrow_exception(eptr);
      }
      catch (const std::exception& ex) {
        err += "std::exception -- ";
        err += ex.what();
      }
      catch (const std::string& message) {
        err += "thrown message -- ";
        err += message.c_str();
      }
      catch (const char* message) {
        err += "thrown message -- ";
        err += message;
      }
      catch (...) {
        err.append("thrown but unknown type, cannot serialize into error message");
      }
    }

    if (type_of(s, res.stack_index()) == sol::type::string) {
      err += ": ";
      sol::string_view serr = sol::stack::get<sol::string_view>(s, res.stack_index());
      err += serr.data();
    }

    printf("-- LUA ERROR --\n\n%s\n\n", err.c_str());
    return false;
  }
  return true;
}

atLua::atLua()
{
  m_pLua = atNew<sol::state>();
  lua_atpanic(*m_pLua, sol::c_call<decltype(&_LuaPanic), &_LuaPanic>);
  m_pLua->set_exception_handler(_ExceptionHandler);
  m_pLua->open_libraries();
}

atLua::~atLua() { atDelete(m_pLua); }
sol::state * atLua::GetLua() { return m_pLua; }
bool atLua::RunText(const atString &command) { return _LuaCall(*m_pLua, m_pLua->script(command.c_str(), &sol::script_pass_on_error)); }
bool atLua::RunFile(const atString &filename) { return _LuaCall(*m_pLua, m_pLua->script_file(filename.c_str(), &sol::script_pass_on_error)); }
