
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

#include "atSceneScript.h"
#include "atScene.h"

static int64_t _nextScriptID = 0;

const int64_t atSceneScript::typeID = atSceneComponent::NextTypeID();

atSceneScript::atSceneScript() : m_scriptID(_nextScriptID++)
{
  m_scriptIDString = atString(m_scriptID); 
}

atSceneScript::~atSceneScript() { Unload(); }

bool atSceneScript::Reload()
{
  if (!Unload())
    return false;
  atLua *pLua = Scene()->GetLua();
  return pLua ? pLua->RunText("atScene.Scripts[\"" + m_scriptIDString + "\"] = dofile([[" + m_path.Path() + "]])") : false;
}

bool atSceneScript::Unload()
{
  atLua *pLua = Scene()->GetLua();
  return pLua ? pLua->RunText("atScene.Scripts[\"" + m_scriptIDString + "\"] = nil") : false;
}

bool atSceneScript::OnCreate() 
{ 
  atLua *pLua = Scene()->GetLua();
  return pLua ? pLua->RunText("atScene.CallScript([[" + m_scriptIDString + "]], [[OnCreate]])") : false;
}

bool atSceneScript::OnDestroy()
{
  atLua *pLua = Scene()->GetLua();
  return pLua ? pLua->RunText("atScene.CallScript([[" + m_scriptIDString + "]], [[OnDestroy]])") : false;
}

bool atSceneScript::OnUpdate(const double dt)
{
  atLua *pLua = Scene()->GetLua();
  return pLua ? pLua->RunText("atScene.CallScript([[" + m_scriptIDString + "]], [[OnUpdate]])") : false;
}

bool atSceneScript::OnDraw(const atMat4D &vp)
{
  atLua *pLua = Scene()->GetLua();
  return pLua ? pLua->RunText("atScene.CallScript([[" + m_scriptIDString + "]], [[OnDraw]])") : false;
}

int64_t atSceneScript::ScriptID() { return m_scriptID; }
const char* atSceneScript::ScriptIDString() { return m_scriptIDString; }

int64_t atSceneScript::TypeID() const { return typeID; }
