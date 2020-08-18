#ifndef atFBXCommon_h__
#define atFBXCommon_h__

#define FBXSDK_SHARED

#include "atFilename.h"
#include "atAssign.h"
#include "../../../3rdParty/fbxsdk/include/fbxsdk.h"

class atFBXCommon
{
public:
  atFBXCommon();
  ~atFBXCommon();

  FbxScene* Import(const atFilename &path);
  bool Export(const atFilename &path, FbxScene *pScene);

  FbxScene* Create();

  FbxManager* GetManager();
  FbxImporter* GetImporter();
  FbxExporter* GetExporter();
protected:
  FbxManager *m_pManager = nullptr;
  FbxImporter *m_pImporter = nullptr;
  FbxExporter *m_pExporter = nullptr;
};

template<> atMat4D& atAssign(atMat4D &lhs, const FbxAMatrix &rhs);
template<> FbxAMatrix& atAssign(FbxAMatrix &lhs, const atMat4D &rhs);

#endif // atFBXCommon_h__
