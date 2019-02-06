#include "atLuaSceneCamera.h"
#include "atSceneCamera.h"
#include "atLuaSceneNode.h"

#define SetVar(name, to) if(IsValid()) Camera()->name to
#define GetVar(name, defaultVal) IsValid() ? Camera()->name : defaultVal;
atLuaSceneNode atLuaSceneCamera::Node() { return GetVar(Node(), atLuaSceneNode()); }
atSceneCamera* atLuaSceneCamera::Camera() { return (atSceneCamera*)m_pComponent; }
bool atLuaSceneCamera::IsValid() { return m_pComponent && m_pComponent->Is<atSceneCamera>(); }

atLuaSceneCamera::atLuaSceneCamera(const atLuaSceneComponent &comp) : atLuaSceneComponent(comp.m_pComponent) {}
atLuaSceneCamera::atLuaSceneCamera(atSceneComponent *pComp) : atLuaSceneComponent(pComp) {}
atLuaSceneCamera::atLuaSceneCamera(const atLuaSceneCamera & copy) : atLuaSceneComponent(copy.m_pComponent) {}
atLuaSceneCamera::atLuaSceneCamera(atLuaSceneCamera && move) : atLuaSceneComponent(move.m_pComponent)  { move.m_pComponent = nullptr; }

double atLuaSceneCamera::GetFOV() { return GetVar(m_fov, 0); }
double atLuaSceneCamera::GetAspect() { return GetVar(m_aspect, 0); }
double atLuaSceneCamera::GetFarPlane() { return GetVar(m_farPlane, 0); }
double atLuaSceneCamera::GetNearPlane() { return GetVar(m_nearPlane, 0); }
atVec2D atLuaSceneCamera::GetDepthRange() { return GetVar(m_depthRange, 0); }
atVec4D atLuaSceneCamera::GetViewport() {  return GetVar(Viewport(), 0); }
void atLuaSceneCamera::SetFOV(const double fov) { SetVar(m_fov, = fov); }
void atLuaSceneCamera::SetAspect(const double aspect) { SetVar(m_aspect, = aspect); }
void atLuaSceneCamera::SetFarPlane(const double farPlane) { SetVar(m_fov, = farPlane); }
void atLuaSceneCamera::SetNearPlane(const double nearPlane) { SetVar(m_fov, = nearPlane); }
void atLuaSceneCamera::SetDepthRange(const atVec2D &range) { SetVar(m_depthRange, = range); }
void atLuaSceneCamera::SetDepthRangeA(const double min, const double max) { SetDepthRange({ min, max }); }
void atLuaSceneCamera::SetViewport(const atVec4I &vp) { SetVar(SetViewport, (vp)); }
void atLuaSceneCamera::SetViewportA(const double x, const double y, const double w, const double h) { SetViewport({x, y, w, h}); }
