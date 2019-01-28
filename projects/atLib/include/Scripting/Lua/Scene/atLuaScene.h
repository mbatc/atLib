#ifndef atLuaScene_h__
#define atLuaScene_h__

#include "atLuaSceneNode.h"
#include "atScene.h"

// Functions are named to reflect there parameters as this
// makes it easier to implement overloaded functions in lua
// as the compiler struggles to resolve overloaded member functions
// when taking a pointer to them

class atLuaScene
{
public:
  atLuaScene(atScene *pScene);
  atLuaScene(const atLuaScene &copy);
  atLuaScene(atLuaScene &&move);
  ~atLuaScene();

  const char* GetName() const;

  int64_t GetRootNodeID() const;
  atLuaSceneNode GetRootNode() const;
  
  atLuaSceneNode GetNode(const int64_t id) const;

  atVector<int64_t> GetNodeIDs() const;

  int64_t CreateNodePRSP(const char *name, const atVec3D &position, const atVec3D &rotation, const atVec3D &scale, const int64_t parentID);
  int64_t CreateNodePRS(const char *name, const atVec3D &position, const atVec3D &rotation, const atVec3D &scale);
  int64_t CreateNodePR(const char *name, const atVec3D &position, const atVec3D &rotation);
  int64_t CreateNodeP(const char *name, const atVec3D &position);
  int64_t CreateNode(const char *name);

  bool DeleteNode(const int64_t id, bool migrateChildren = false);
  bool DeleteNodeFromID(const atLuaSceneNode &node, bool migrateChildren = false);

  bool AddActiveCamera(const int64_t id);
  bool RemoveActiveCamera(const int64_t id);

protected:
  atScene *m_pScene;
};

#endif // atLuaScene_h__
