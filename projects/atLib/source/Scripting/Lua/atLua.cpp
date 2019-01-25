
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
