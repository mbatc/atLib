#include "atLuaSceneComponent.h"
#include "atLuaSceneNode.h"

atLuaSceneComponent::atLuaSceneComponent(atSceneComponent *pComp) : m_pComponent(pComp) {}
atLuaSceneComponent::atLuaSceneComponent(const atLuaSceneComponent &copy) : m_pComponent(copy.m_pComponent) {}
atLuaSceneComponent::atLuaSceneComponent(atLuaSceneComponent &&move) : m_pComponent(move.m_pComponent) { move.m_pComponent = nullptr; }
atLuaSceneComponent::~atLuaSceneComponent() { m_pComponent = nullptr; }

const atLuaSceneComponent& atLuaSceneComponent::operator=(const atLuaSceneComponent & rhs) { m_pComponent = rhs.m_pComponent; return *this; }
const atLuaSceneComponent& atLuaSceneComponent::operator=(atLuaSceneComponent && rhs) { m_pComponent = rhs.m_pComponent; rhs.m_pComponent = nullptr; return *this; }

atLuaSceneNode atLuaSceneComponent::Node() { return m_pComponent ? m_pComponent->Node() : atLuaSceneNode(); }
int64_t atLuaSceneComponent::TypeID() const { return m_pComponent ? m_pComponent->TypeID() : -1; }
bool atLuaSceneComponent::IsValid() const { return m_pComponent != nullptr; }