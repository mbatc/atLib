#include "atNode.h"

bool atNode::SetParent(atNode *pParent) { return m_pParent = pParent; }
atNode* atNode::Parent() { return m_pParent; }
atNode* atNode::Root() { return m_pRoot; }
atNode* atNode::Child(const int64_t index) { return m_children[index]; }
int64_t atNode::ChildCount() { return m_children.size(); }
const atVector<atNode*>& atNode::Children() { return m_children; }

bool atNode::RemoveParent()
{
  if (!m_pParent)
    return false;

  m_pParent = nullptr;
  return true;
}

bool atNode::AddChild(atNode *pChild)
{
  for (atNode *pNode : m_children)
    if (pChild == pNode)
      return false;
  m_children.push_back(pChild);
  return true;
}

bool atNode::RemoveChild(atNode *pChild)
{
  for (int64_t i = 0; i < m_children.size(); ++i)
    if (pChild == m_children[i])
      return RemoveChild(i);
  return false;
}

bool atNode::RemoveChild(const int64_t index)
{
  if (index < 0 || index >= m_children.size())
    return false;
  m_children.erase(index);
  return true;
}
