#ifndef atHierarchy_h__
#define atHierarchy_h__

#include "atTypes.h"

static const int64_t atHierarchy_RootNodeID = -1;

// ----------------------------------------------------------------------- //
// Example Visitor - Prints nodes in a atHierarchy<atString>               //
//                                                                         //
// ----------------------------------------------------------------------- //
//                                                                         //
// class PrintVisitor                                                      //
// {                                                                       //
// public:                                                                 //
//   void Leave(const int64_t &nodeID, atHierarchy<atString> *pHierarchy)  //
//   {                                                                     //
//     padding = padding.substr(0, padding.length() - 3);                  //
//   }                                                                     //
//                                                                         //
//   bool Visit(const int64_t &nodeID, atHierarchy<atString> *pHierarchy)  //
//   {                                                                     //
//     printf(padding + "|\n");                                            //
//     atString node = pHierarchy->Get(nodeID);                            //
//     atString outstr = padding + "+-- " + node + "\n";                   //
//     printf(outstr);                                                     //
//     padding += "|  ";                                                   //
//     return true;                                                        //
//   }                                                                     //
//                                                                         //
//   atString padding = "";                                                //
// };                                                                      //
//                                                                         //
// ----------------------------------------------------------------------- //

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
  void MoveChildren(const int64_t &nodeID, const int64_t &toNodeID);

  // Move a node to a new parent node
  void Move(const int64_t &nodeID, const int64_t &toNodeID);

  // Get a node from the tree
  T& Get(const int64_t &nodeID);
  const T& Get(const int64_t &nodeID) const;
  
  // Get the root node
  T& Root();
  const T& Root() const;

  // Get a nodes children
  atVector<int64_t> Children(const int64_t &nodeID);

  // Visit nodes in the hierarchy. A valid visitor must define the function
  // Visit(const int64_t &nodeID, atHierarchy<T> *pHierarchy);
  //  - This function should return true to continue down the tree or false to terminate visiting a branch
  template<typename T2> void Visit(T2 *pVisitor, const int64_t &nodeID = atHierarchy_RootNodeID);

  // Visit nodes in the hierarchy using a lambda function
  void Visit(const std::function<bool(int64_t, atHierarchy<T>*)> &visitor, const int64_t &nodeID = atHierarchy_RootNodeID, const std::function<void(int64_t, atHierarchy<T>*)> &leave = nullptr);

  // Visit parent nodes in the hierarchy using a lambda function
  void VisitParent(const std::function<bool(int64_t, atHierarchy<T>*)> &visitor, const int64_t &nodeID, const std::function<void(int64_t, atHierarchy<T>*)> &leave = nullptr);

  atHierarchy<T>& operator=(const atHierarchy<T> &rhs);
  atHierarchy<T>& operator=(atHierarchy<T> &&rhs);

protected:
  void RemoveChildID(const int64_t &parentID, const int64_t &nodeID);
  void AddChildID(const int64_t &parentID, const int64_t &nodeID);
  int64_t Add(Node &&node, const int64_t &parentID);

  Node* GetNode(const int64_t &nodeID);

  int64_t GetNextID();

  atHashMap<int64_t, Node> m_nodes;
  atHashMap<int64_t, int64_t> m_parents;

  int64_t m_nextID = 0;
};

#include "atHierarchy.inl"

#endif // atHierarchy_h__
