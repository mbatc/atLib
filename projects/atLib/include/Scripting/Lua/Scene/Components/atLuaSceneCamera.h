#ifndef atLuaSceneCamera_h__
#define atLuaSceneCamera_h__

#include "atLuaSceneComponent.h"

class atSceneCamera;

class atLuaSceneCamera : public atLuaSceneComponent
{
public:
  atLuaSceneCamera(const atLuaSceneComponent &comp);
  atLuaSceneCamera(atSceneComponent *pComp);
  atLuaSceneCamera(const atLuaSceneCamera &copy);
  atLuaSceneCamera(atLuaSceneCamera &&move);

  double GetFOV();
  double GetAspect();
  double GetFarPlane();
  double GetNearPlane();
  atVec2D GetDepthRange();
  atVec4D GetViewport();

  void SetFOV(const double fov);
  void SetAspect(const double aspect);
  void SetFarPlane(const double farPlane);
  void SetNearPlane(const double nearPlane);
  void SetDepthRange(const atVec2D &range);
  void SetDepthRangeA(const double min, const double max);
  void SetViewport(const atVec4I &vp);
  void SetViewportA(const double x, const double y, const double w, const double h);

  atSceneCamera* Camera();
  bool IsValid();
};

#endif // atLuaSceneCamera_h__
