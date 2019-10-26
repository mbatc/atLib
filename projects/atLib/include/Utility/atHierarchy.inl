
template<typename T>
inline atHierarchy<T>::atHierarchy() { Clear(); }

template<typename T>
inline void atHierarchy<T>::Clear()
{
  m_nodes.Clear();
  m_parents.Clear();
  m_nextID = 0;
  m_nodes.Add(atHierarchy_atRootNodeID, {});
}

template <typename T>
inline int64_t atHierarchy<T>::Add(const T &node, const int64_t &parentID)
{
  Node newNode;
  newNode.node = node;
  return Add(std::move(newNode), parentID);
}

template<typename T>
inline int64_t atHierarchy<T>::Add(T &&node, const int64_t &parentID)
{
  Node newNode;
  newNode.node = std::move(node);
  return Add(std::move(newNode), parentID);
}

template<typename T>
inline void atHierarchy<T>::Remove(const int64_t &nodeID)
{
  // Remove node recursively
  Node *pNode = GetNode(nodeID);
  for (int64_t i = 0; i < pNode->children.size(); ++i)
  {
    m_parents.Remove(pNode->children[i]);
    Remove(std::copy(pNode->children[i]));
  }

  // Remove reference to node from parent
  int64_t *pParentID = m_parents.TryGet(nodeID);
  if (pParentID)
    RemoveChildID(*pParentID, nodeID);

  // Delete node
  m_nodes.Remove(nodeID);
}

template<typename T>
inline void atHierarchy<T>::MoveChildren(const int64_t &nodeID, const int64_t &toNodeID)
{
  if (nodeID == atHierarchy_atRootNodeID)
    return;

  for (int64_t id : Children(nodeID))
    Move(id, toNodeID);
}

template<typename T>
inline void atHierarchy<T>::Move(const int64_t &nodeID, const int64_t &toNodeID)
{
  if (nodeID == atHierarchy_atRootNodeID)
    return;

  RemoveChildID(m_parents[nodeID], nodeID);
  AddChildID(toNodeID, nodeID);
}

template<typename T>
inline T& atHierarchy<T>::Get(const int64_t &nodeID) { return m_nodes[nodeID].node; }

template<typename T>
inline const T& atHierarchy<T>::Get(const int64_t &nodeID) const { return m_nodes[nodeID].node; }

template<typename T>
inline T & atHierarchy<T>::Root()
{
  return Get(atHierarchy_atRootNodeID);
}

template<typename T>
inline const T & atHierarchy<T>::Root() const
{
  return Get(atHierarchy_atRootNodeID);
}

template<typename T>
inline atVector<int64_t> atHierarchy<T>::Children(const int64_t &nodeID)
{
  Node *pNode = GetNode(nodeID);
  return pNode ? pNode->children : atVector<int64_t>();
}

template<typename T>
template<typename T2>
inline void atHierarchy<T>::Visit(T2 *pVisitor, const int64_t &nodeID = atHierarchy_atRootNodeID)
{
  if (pVisitor->Visit(nodeID, this))
    for (int64_t childID : Children(nodeID))
      Visit(pVisitor, childID);
  pVisitor->Leave(nodeID, this);
}

template<typename T>
inline void atHierarchy<T>::RemoveChildID(const int64_t &parentID, const int64_t &nodeID)
{
  Node *pParent = GetNode(parentID);
  for (int64_t i = 0; i < pParent->children.size(); ++i)
    if (pParent->children[i] == nodeID)
    {
      pParent->children.erase(i);
      break;
    }

  m_parents.Remove(nodeID);
}

template<typename T>
inline void atHierarchy<T>::AddChildID(const int64_t &parentID, const int64_t &nodeID)
{
  Node *pParent = GetNode(parentID);
  pParent->children.push_back(nodeID);
  if (!m_parents.TryAdd(nodeID, parentID))
    m_parents[nodeID] = parentID;
}

template<typename T>
inline int64_t atHierarchy<T>::Add(Node &&node, const int64_t &parentID)
{
  int64_t id = GetNextID();
  m_nodes.Add(id, std::move(node));
  AddChildID(parentID, id);
  return id;
}

template<typename T>
inline typename atHierarchy<T>::Node* atHierarchy<T>::GetNode(const int64_t &nodeID) { return m_nodes.TryGet(nodeID); }

template<typename T>
inline int64_t atHierarchy<T>::GetNextID() { return m_nextID++; }