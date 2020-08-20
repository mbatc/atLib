#ifndef atHierarchy_h__
#define atHierarchy_h__

#include "atTypes.h"
#include "atHashMap.h"

static const int64_t atHierarchy_RootNodeID = 0;
static const int64_t atHierarchy_InvalidNodeID = AT_INVALID_ID;

template <typename T> class atHierarchy
{
protected:
  struct Node
  {
    T node;
    atVector<int64_t> children;
  };

public:
  atHierarchy();
  atHierarchy(const atHierarchy<T> &o);
  atHierarchy(atHierarchy<T> &&o);
  
  // Clear all nodes (excluding the root node)
  void Clear();

  // Get the IDs of every node in the hierarchy
  atVector<int64_t> GetIDs() const;

  // Add a new node to the tree.
  // Copy construct into the node.
  int64_t Add(const T &node, const int64_t &parentID = atHierarchy_RootNodeID);

  // Add a new node to the tree.
  // Move construct into the node.
  int64_t Add(T &&node, const int64_t &parentID = atHierarchy_RootNodeID);

  // Check if the hierarchy contains a node with the specified ID
  bool Contains(const int64_t &nodeID) const;

  // Remove a node from the tree
  // Removes all child nodes as well
  void Remove(const int64_t &nodeID);

  // Moves all child nodes to a new parent
  bool MoveChildren(const int64_t &nodeID, const int64_t &toNodeID, const bool &persistOnError);

  // Move a node to a new parent node
  bool Move(const int64_t &nodeID, const int64_t &toNodeID);

  // Check if 'testID' if can be reach by traversing down the
  // tree from 'parentID'
  bool IsDecendant(const int64_t &testID, const int64_t &parentID) const;

  // Get a node from the tree
  T& Get(const int64_t &nodeID);
  const T& Get(const int64_t &nodeID) const;
  
  // Get the root node
  T& Root();
  const T& Root() const;

  // Get a nodes children
  atVector<int64_t> Children(const int64_t &nodeID) const;

  // Get a nodes parent ID
  int64_t GetParent(const int64_t &nodeID) const;

  // Visit nodes in the hierarchy using a lambda function
  void Visit(const std::function<bool(int64_t, atHierarchy<T>*)> &visitor, const int64_t &nodeID = atHierarchy_RootNodeID, const std::function<void(int64_t, atHierarchy<T>*)> &leave = nullptr);

  // Visit parent nodes in the hierarchy using a lambda function
  void VisitParent(const std::function<bool(int64_t, atHierarchy<T>*)> &visitor, const int64_t &nodeID, const std::function<void(int64_t, atHierarchy<T>*)> &leave = nullptr);

  // Visit nodes in the hierarchy using a lambda function
  void Visit(const std::function<bool(int64_t, const atHierarchy<T>*)> &visitor, const int64_t &nodeID = atHierarchy_RootNodeID, const std::function<void(int64_t, const atHierarchy<T>*)> &leave = nullptr) const;

  // Visit parent nodes in the hierarchy using a lambda function
  void VisitParent(const std::function<bool(int64_t, const atHierarchy<T>*)> &visitor, const int64_t &nodeID, const std::function<void(int64_t, const atHierarchy<T>*)> &leave = nullptr) const;

  int64_t Find(const std::function<bool(int64_t, const atHierarchy<T>*)> &matcher, const int64_t &rootID = atHierarchy_RootNodeID) const;
  atVector<int64_t> FindAll(const std::function<bool(int64_t, const atHierarchy<T>*)> &matcher, const int64_t &rootID = atHierarchy_RootNodeID) const;

  atHierarchy<T>& operator=(const atHierarchy<T> &rhs);
  atHierarchy<T>& operator=(atHierarchy<T> &&rhs);

protected:
  void RemoveChildID(const int64_t &parentID, const int64_t &nodeID);
  void AddChildID(const int64_t &parentID, const int64_t &nodeID);
  int64_t Add(Node &&node, const int64_t &parentID);

  Node* GetNode(const int64_t &nodeID);
  const Node* GetNode(const int64_t &nodeID) const;

  int64_t GetNextID();

  atHashMap<int64_t, Node> m_nodes;
  atHashMap<int64_t, int64_t> m_parents;

  int64_t m_nextID = atHierarchy_RootNodeID + 1;
};

#include "atHierarchy.inl"

#endif // atHierarchy_h__
