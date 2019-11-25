
template <typename T> class atHierarchyExtractor
{
public:
  atHierarchyExtractor(atHierarchy<T> *pTarget, const int64_t &insertAtNode = atHierarchy_atRootNodeID, const bool &move = false)
  {
    m_pTarget = pTarget;
    m_move = move;
    m_lastParent.push_back(insertAtNode);
  }

  void Leave(const int64_t &nodeID, atHierarchy<T> *pHierarchy)
  {
    if (m_move)
      m_pTarget->Remove(m_lastParent.back());

    m_lastParent.pop_back();
  }

  bool Visit(const int64_t &nodeID, atHierarchy<T> *pHierarchy)
  {
    if (!m_pTarget)
      return false;

    T *pNode = pHierarchy->Get(nodeID);
    int64_t newID = atHierarchy_atRootNodeID;
    if (m_move)
      newID = m_pTarget->Add(std::move(*pNode), m_lastParent.back());
    else
      newID = m_pTarget->Add(*pNode, m_lastParent.back());

    if (m_lastParent.size() == 1)
      m_insertedID = newID;

    m_lastParent.push_back(newID);
    return true;
  }

  int64_t m_insertedID = atHierarchy_atRootNodeID;

protected:
  atVector<int64_t> m_lastParent;
  atHierarchy<T> *m_pTarget = nullptr;
  bool m_move = false;
};

template<typename T>
inline atHierarchy<T>::atHierarchy() { Clear(); }

template<typename T>
inline atHierarchy<T>::atHierarchy(atHierarchy<T> &&o) { *this = std::move(o); }

template<typename T>
inline atHierarchy<T>::atHierarchy(const atHierarchy<T> &o) { *this = o; }

template<typename T>
inline atHierarchy<T>& atHierarchy<T>::operator=(atHierarchy<T> &&o)
{
  Clear();
  m_nodes = std::move(o.m_nodes);
  m_nextID = std::move(o.m_nextID);
  m_parents = std::move(o.m_parents);
  m_idLookup = std::move(o.m_idLookup);
  o.Clear();
  return *this;
}

template<typename T>
inline atHierarchy<T>& atHierarchy<T>::operator=(const atHierarchy<T> &o)
{
  Clear();
  // atHierarchyConstExtractor<T> extractor(this);
  // o.Visit(&extractor);
  return *this;
}

template<typename T>
inline void atHierarchy<T>::Clear()
{
  m_nodes.Clear();
  m_parents.Clear();
  m_idLookup.Clear();
  m_nextID = 0;

  Node root;
  root.pNode = atNew<T>();
  m_nodes.Add(atHierarchy_atRootNodeID, std::move(root));
  m_idLookup.Add((void*)root.pNode, atHierarchy_atRootNodeID);
}

template <typename T>
inline int64_t atHierarchy<T>::Add(const T &node, const int64_t &parentID)
{
  Node newNode;
  newNode.pNode = atNew<T>(node);
  return Add(std::move(newNode), parentID);
}

template<typename T>
inline int64_t atHierarchy<T>::Add(T &&node, const int64_t &parentID)
{
  Node newNode;
  newNode.pNode = atNew<T>(std::move(node));
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
    Remove(int64_t(pNode->children[i]));
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
inline atVector<int64_t> atHierarchy<T>::Children(const int64_t &nodeID) const
{
  const Node *pNode = GetNode(nodeID);
  return pNode ? pNode->children : atVector<int64_t>();
}

template<typename T>
inline int64_t atHierarchy<T>::Parent(const int64_t &nodeID) const
{
  const int64_t *pParentID = m_parents.TryGet(nodeID);
  return pParentID ? *pParentID : atHierarchy_atRootNodeID;
}

template<typename T>
inline int64_t atHierarchy<T>::GetID(const T *pNode) const { return m_idLookup.Contains((void*)pNode) ? m_idLookup[(void*)pNode] : -1; }

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
inline atHierarchy<T> atHierarchy<T>::Extract(const int64_t &nodeID, const bool &move)
{
  atHierarchy<T> tree;
  atHierarchyExtractor<T> extractor(&tree, atHierarchy_atRootNodeID, move);
  Visit(&extractor, nodeID);
  return tree;
}

template<typename T>
inline int64_t atHierarchy<T>::Insert(atHierarchy<T> *pTree, const int64_t &parentID, const bool &move)
{
  atHierarchyExtractor<T> extractor(this, parentID);
  pTree->Visit(&extractor);
  return extractor.m_insertedID;
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
  if (!pParent)
    return;

  pParent->children.push_back(nodeID);
  if (!m_parents.TryAdd(nodeID, parentID))
    m_parents[nodeID] = parentID;
}

template<typename T>
inline int64_t atHierarchy<T>::Add(Node &&node, const int64_t &parentID)
{
  int64_t id = GetNextID();
  m_idLookup.Add((void*)node.pNode, id);
  m_nodes.Add(id, std::move(node));
  AddChildID(parentID, id);
  return id;
}

template<typename T>
inline T* atHierarchy<T>::Get(const int64_t &nodeID) { return m_nodes[nodeID].pNode; }

template<typename T>
inline const T* atHierarchy<T>::Get(const int64_t &nodeID) const { return m_nodes[nodeID].pNode; }

template<typename T>
inline T* atHierarchy<T>::Child(const int64_t &nodeID, const int64_t &index) { return Get(GetNode(nodeID)->children[index]); }

template<typename T>
inline const T* atHierarchy<T>::Child(const int64_t &nodeID, const int64_t &index) const { return Get(GetNode(nodeID)->children[index]); }

template<typename T>
inline T* atHierarchy<T>::Root() { return Get(atHierarchy_atRootNodeID); }

template<typename T>
inline const T* atHierarchy<T>::Root() const { return Get(atHierarchy_atRootNodeID); }

template<typename T>
inline int64_t atHierarchy<T>::ChildCount(const int64_t &nodeID) const { return GetNode(nodeID)->children.size(); }

template<typename T>
inline typename atHierarchy<T>::Node* atHierarchy<T>::GetNode(const int64_t &nodeID) { return m_nodes.TryGet(nodeID); }

template<typename T>
inline typename const atHierarchy<T>::Node* atHierarchy<T>::GetNode(const int64_t &nodeID) const { return m_nodes.TryGet(nodeID); }

template<typename T>
inline int64_t atHierarchy<T>::GetNextID() { return m_nextID++; }