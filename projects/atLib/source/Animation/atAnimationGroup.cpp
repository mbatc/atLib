#include "atAnimationGroup.h"

const atString& atAnimationGroup::GetName() const { return m_name; }
void atAnimationGroup::SetName(const atString &name) { m_name = name; }
int64_t atAnimationGroup::AddAnimation() { return m_anims.emplace(); }

atAnimation* atAnimationGroup::GetAnimation(const int64_t &animID) { return m_anims.Contains(animID) ? &m_anims[animID] : nullptr; }
bool atAnimationGroup::RemoveAnimation(const int64_t &animID) { return m_anims.erase(animID); }
