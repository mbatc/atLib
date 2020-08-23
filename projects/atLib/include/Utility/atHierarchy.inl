
template<typename T>
inline atHierarchy<T>::atHierarchy() { Clear(); }

template<typename T>
inline atHierarchy<T>::atHierarchy(const atHierarchy<T> &o) { *this = o; }

template<typename T>
inline atHierarchy<T>::atHierarchy(atHierarchy<T> &&o) { *this = std::move(o); }

template<typename T>
inline void atHierarchy<T>::Clear()
{
  m_nodes.Clear();
  m_parents.Clear();
  m_nextID = atHierarchy_RootNodeID + 1;
  m_nodes.Add(atHierarchy_RootNodeID, {});
}

template<typename T>
inline atVector<int64_t> atHierarchy<T>::GetIDs() const
{
  return m_nodes.GetKeys();
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
inline bool atHierarchy<T>::Contains(const int64_t &nodeID) const { return m_nodes.Contains(nodeID); }

template<typename T>
inline bool atHierarchy<T>::Remove(const int64_t &nodeID)
{
  // Remove node recursively
  Node *pNode = GetNode(nodeID);
  if (!pNode)
    return false;

  for (int64_t i = 0; i < pNode->children.size(); ++i)
  {
    m_parents.Remove(pNode->children[i]);
    Remove(int64_t(pNode->children[i]));
  }

  // Remove reference to node from parent
  int64_t *pParentID = m_parents.TryGet(nodeID);
  if (pParentID)
    RemoveChildID(*pParentID, nodeID);

  // Delete node
  return m_nodes.Remove(nodeID);
}

template<typename T>
inline bool atHierarchy<T>::MoveChildren(const int64_t &nodeID, const int64_t &toNodeID, const bool &persistOnError)
{
  if (nodeID == atHierarchy_RootNodeID)
    return;

  bool success = true;
  bool rollback = false;
  atVector<int64_t> children = Children(nodeID);
  for (const int64_t &id : children)
  {
    success &= Move(id, toNodeID);
    rollback = !success && !persistOnError;
    if (rollback)
      break;
  }

  if (rollback)
  { // Move children back to the original parent
    for (const int64_t &id : children)
      Move(id, nodeID);
  }

  return success;
}

template<typename T>
inline bool atHierarchy<T>::Move(const int64_t &nodeID, const int64_t &toNodeID)
{
  if (nodeID == atHierarchy_RootNodeID || IsDecendant(toNodeID, nodeID))
    return false;

  RemoveChildID(m_parents[nodeID], nodeID);
  AddChildID(toNodeID, nodeID);
  return true;
}

template<typename T>
inline bool atHierarchy<T>::IsDecendant(const int64_t &testID, const int64_t &parentID) const
{
  bool found = false;
  VisitParent([&found, parentID](int64_t nodeID, const atHierarchy<T>* /*pTree*/) {
    found |= parentID == nodeID;
    return !found; // Continue until we find a parent or reach the top of the tree
  }, testID);

  return found;
}

template<typename T>
inline T& atHierarchy<T>::Get(const int64_t &nodeID) { return m_nodes[nodeID].node; }

template<typename T>
inline const T& atHierarchy<T>::Get(const int64_t &nodeID) const { return m_nodes[nodeID].node; }

template<typename T>
inline T& atHierarchy<T>::Root()
{
  return Get(atHierarchy_RootNodeID);
}

template<typename T>
inline const T& atHierarchy<T>::Root() const
{
  return Get(atHierarchy_RootNodeID);
}

template<typename T>
inline atVector<int64_t> atHierarchy<T>::Children(const int64_t &nodeID) const
{
  const Node *pNode = GetNode(nodeID);
  return pNode ? pNode->children : atVector<int64_t>();
}

template<typename T>
inline int64_t atHierarchy<T>::GetParent(const int64_t &nodeID) const
{
  const int64_t *pParent = m_parents.TryGet(nodeID);
  return pParent ? *pParent : atHierarchy_InvalidNodeID;
}

template<typename T>
inline void atHierarchy<T>::Visit(const std::function<bool(int64_t, atHierarchy<T>*)> &visitor, const int64_t &nodeID, const std::function<void(int64_t, atHierarchy<T>*)> &leave)
{
  if (visitor(nodeID, this))
  {
    for (const int64_t &childID : Children(nodeID))
      Visit(visitor, childID);

    if (leave)
      leave(nodeID, this);
  }
}

template<typename T>
inline void atHierarchy<T>::VisitParent(const std::function<bool(int64_t, atHierarchy<T>*)> &visitor, const int64_t &nodeID, const std::function<void(int64_t, atHierarchy<T>*)> &leave)
{
  if (visitor(nodeID, this))
  {
    int64_t *pParent = m_parents.TryGet(nodeID);
    if (pParent)
      VisitParent(visitor, *pParent, leave);

    if (leave)
      leave(nodeID, this);
  }
}

template<typename T>
inline void atHierarchy<T>::Visit(const std::function<bool(int64_t, const atHierarchy<T>*)> &visitor, const int64_t &nodeID, const std::function<void(int64_t, const atHierarchy<T>*)> &leave) const
{
  if (visitor(nodeID, this))
  {
    for (const int64_t &childID : Children(nodeID))
      Visit(visitor, childID);

    if (leave)
      leave(nodeID, this);
  }
}

template<typename T>
inline void atHierarchy<T>::VisitParent(const std::function<bool(int64_t, const atHierarchy<T>*)> &visitor, const int64_t &nodeID, const std::function<void(int64_t, const atHierarchy<T>*)> &leave) const
{
  if (visitor(nodeID, this))
  {
    const int64_t *pParent = m_parents.TryGet(nodeID);
    if (pParent)
      VisitParent(visitor, *pParent, leave);

    if (leave)
      leave(nodeID, this);
  }
}

template<typename T>
inline int64_t atHierarchy<T>::Find(const std::function<bool(int64_t, const atHierarchy<T>*)> &matcher, const int64_t &rootID) const
{
  int64_t foundID = atHierarchy_RootNodeID;
  Visit([&foundID, matcher](int64_t nodeID, const atHierarchy<T> *pTree) {
    if (!matcher(nodeID, pTree))
      return true;
    foundID = nodeID;
    return false;
  });

  return foundID;
}

template<typename T>
inline atVector<int64_t> atHierarchy<T>::FindAll(const std::function<bool(int64_t, const atHierarchy<T>*)> &matcher, const int64_t &rootID) const
{
  atVector<int64_t> foundIDs;
  Visit([&foundID, matcher](int64_t nodeID, const atHierarchy<T> *pTree) {
    if (matcher(nodeID, pTree))
      foundIDs.push_back(nodeID);
    return true;
  });
  return foundIDs;
}

template<typename T>
inline atHierarchy<T>& atHierarchy<T>::operator=(const atHierarchy<T> &rhs)
{
  m_nodes = rhs.m_nodes;
  m_parents = rhs.m_parents;
  m_nextID = rhs.m_nextID;
  return *this;
}

template<typename T>
inline atHierarchy<T>& atHierarchy<T>::operator=(atHierarchy<T> &&rhs)
{
  m_nodes = std::move(rhs.m_nodes);
  m_parents = std::move(rhs.m_parents);
  m_nextID = rhs.m_nextID;
  rhs.m_nextID = 0;
  return *this;
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
inline const typename atHierarchy<T>::Node* atHierarchy<T>::GetNode(const int64_t & nodeID) const { return m_nodes.TryGet(nodeID); }

template<typename T>
inline int64_t atHierarchy<T>::GetNextID() { return m_nextID++; }