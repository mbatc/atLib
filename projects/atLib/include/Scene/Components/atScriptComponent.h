#ifndef atScriptComponent_h__
#define atScriptComponent_h__

#include "atSceneComponent.h"

class atScriptComponent;

typedef bool(*ScriptComponent_InitCallback)(atScriptComponent *pComponent);
typedef bool(*ScriptComponent_UpdateCallback)(atScriptComponent *pComponent, const double &deltaTime);
typedef bool(*ScriptComponent_RenderCallback)(atScriptComponent *pComponent, const atMat4D &vp);
typedef bool(*ScriptComponent_DestroyCallback)(atScriptComponent *pComponent);

class atScriptComponent : public atSceneComponent
{
public:
  atScriptComponent(const atString &name);

  bool Init() override;
  bool Update(const double &deltaTime) override;
  bool Render(const atMat4D &vp) override;
  bool Destroy() override;

  atString m_script;

  // Use these callbacks to setup the external script calls
  // The script component will just call these functions and parse itself through
  static ScriptComponent_InitCallback InitCallback;
  static ScriptComponent_UpdateCallback UpdateCallback;
  static ScriptComponent_RenderCallback RenderCallback;
  static ScriptComponent_DestroyCallback DestroyCallback;

  static atCoreComponentType TypeID();

  atSceneComponent* Copy() const override;
};

#endif // atScriptComponent_h__
