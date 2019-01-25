#include "atSceneComponent.h"
#include "atScene.h"

int64_t atSceneComponent::m_nextTypeID = 0;
int64_t atSceneComponent::NextTypeID() { return 1ll << m_nextTypeID++; }
atScene* atSceneComponent::Scene() { return Node()->Scene(); }
atSceneNode* atSceneComponent::Node() { return m_pNode; }
