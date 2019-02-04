#include "atLuaSceneMeshRenderable.h"

#include "atSceneMeshRenderable.h"

#define SetVar(name, to) if(IsValid()) Mesh()->name to
#define GetVar(name, defaultVal) IsValid() ? Mesh()->name : defaultVal;
atSceneMeshRenderable* atLuaSceneMeshRenderable::Mesh() { return (atSceneMeshRenderable*)m_pComponent; }
bool atLuaSceneMeshRenderable::IsValid() { return m_pComponent && m_pComponent->Is<atSceneMeshRenderable>(); }

atLuaSceneMeshRenderable::atLuaSceneMeshRenderable(const atLuaSceneComponent &comp) : atLuaSceneComponent(comp.m_pComponent) {}
atLuaSceneMeshRenderable::atLuaSceneMeshRenderable(atSceneComponent *pComp) : atLuaSceneComponent(pComp) {}
atLuaSceneMeshRenderable::atLuaSceneMeshRenderable(const atLuaSceneMeshRenderable &copy) : atLuaSceneComponent(copy.m_pComponent) {}
atLuaSceneMeshRenderable::atLuaSceneMeshRenderable(atLuaSceneMeshRenderable &&move) : atLuaSceneComponent(move.m_pComponent) { move.m_pComponent = nullptr; }

const char* atLuaSceneMeshRenderable::GetModelPath() { return GetVar(GetModelPath(), ""); }
bool atLuaSceneMeshRenderable::SetModelPath(const char *file) { return GetVar(SetModel(file), false); }
