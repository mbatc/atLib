#ifndef atLuaSceneScript_h__
#define atLuaSceneScript_h__

#include "atLuaSceneComponent.h"

class atSceneScript;

class atLuaSceneScript : public atLuaSceneComponent
{
public:
  atLuaSceneScript(const atLuaSceneComponent &comp);
  atLuaSceneScript(atSceneComponent *pComp);
  atLuaSceneScript(const atLuaSceneScript &copy);
  atLuaSceneScript(atLuaSceneScript &&move);

  const char* GetScriptSrc();
  const char* GetScriptPath();

  bool Reload();
  bool Unload();

  int64_t ScriptID();
  const char* ScriptIDString();

  void SetScriptPath(const char *path);
  void SetScriptSrc(const char *src);

  atLuaSceneNode Node();
  bool IsValid();
  atSceneScript* Script();
};

#endif // atLuaSceneScript_h__
