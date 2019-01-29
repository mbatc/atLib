#include "atLuaSceneComponent.h"

atLuaSceneComponent::atLuaSceneComponent(atSceneComponent *pComp) : m_pComponent(pComp) {}
atLuaSceneComponent::atLuaSceneComponent(const atLuaSceneComponent &copy) : m_pComponent(copy.m_pComponent) {}
atLuaSceneComponent::atLuaSceneComponent(atLuaSceneComponent &&move) : m_pComponent(move.m_pComponent) { move.m_pComponent = nullptr; }
atLuaSceneComponent::~atLuaSceneComponent() { m_pComponent = nullptr; }

const atLuaSceneComponent& atLuaSceneComponent::operator=(const atLuaSceneComponent & rhs) { m_pComponent = rhs.m_pComponent; return *this; }
const atLuaSceneComponent& atLuaSceneComponent::operator=(atLuaSceneComponent && rhs) { m_pComponent = rhs.m_pComponent; rhs.m_pComponent = nullptr; return *this; }

int64_t atLuaSceneComponent::TypeID() const { return m_pComponent->TypeID(); }
