#include "atLuaSceneComponent.h"

atLuaSceneComponent::atLuaSceneComponent(atSceneComponent *pComp) : m_pComponent(pComp) {}
atLuaSceneComponent::atLuaSceneComponent(const atLuaSceneComponent &copy) : m_pComponent(copy.m_pComponent) {}
atLuaSceneComponent::atLuaSceneComponent(atLuaSceneComponent &&move) : m_pComponent(move.m_pComponent) { move.m_pComponent = nullptr; }
atLuaSceneComponent::~atLuaSceneComponent() { m_pComponent = nullptr; }

int64_t atLuaSceneComponent::TypeID() const { return m_pComponent->TypeID(); }
