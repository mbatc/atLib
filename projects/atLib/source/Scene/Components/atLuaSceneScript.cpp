#include "atLuaSceneScript.h"
#include "atSceneScript.h"
#include "atLuaSceneNode.h"

#define GetVar(name, defaultVal) IsValid() ? Script()->name : defaultVal
#define SetVar(name, to) if(IsValid()) Script()->name to
atSceneScript * atLuaSceneScript::Script() { return (atSceneScript*)m_pComponent; }
atLuaSceneNode atLuaSceneScript::Node() { return GetVar(Node(), atLuaSceneNode()); }
bool atLuaSceneScript::IsValid() { return m_pComponent && m_pComponent->Is<atSceneScript>(); }


atLuaSceneScript::atLuaSceneScript(const atLuaSceneComponent &comp) : atLuaSceneComponent(comp.m_pComponent) {}
atLuaSceneScript::atLuaSceneScript(atSceneComponent *pComp) : atLuaSceneComponent(pComp) {}
atLuaSceneScript::atLuaSceneScript(const atLuaSceneScript &copy) : atLuaSceneComponent(copy.m_pComponent) {}
atLuaSceneScript::atLuaSceneScript(atLuaSceneScript && move) : atLuaSceneComponent(move.m_pComponent) { move.m_pComponent = nullptr; }

const char* atLuaSceneScript::GetScriptSrc() { return GetVar(m_src, ""); }
const char* atLuaSceneScript::GetScriptPath() { return GetVar(m_path.c_str(), ""); }
bool atLuaSceneScript::Reload() { return GetVar(Reload(), false); }
bool atLuaSceneScript::Unload() { return GetVar(Unload(), false); }
int64_t atLuaSceneScript::ScriptID() { return GetVar(ScriptID(), AT_INVALID_ID); }
const char* atLuaSceneScript::ScriptIDString() { return GetVar(ScriptIDString(), "-1"); }
void atLuaSceneScript::SetScriptPath(const char *path) { SetVar(m_path, =path); }
void atLuaSceneScript::SetScriptSrc(const char *src) { SetVar(m_path, =src); }
