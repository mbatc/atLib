#include "atFBXCommon.h"

atFBXCommon::atFBXCommon()
{
  m_pManager = FbxManager::Create();
  FbxIOSettings *pIOS = FbxIOSettings::Create(m_pManager, IOSROOT);
  m_pManager->SetIOSettings(pIOS);
}

atFBXCommon::~atFBXCommon()
{
  m_pManager->Destroy();
}

FbxScene* atFBXCommon::Import(const atFilename &path)
{
  FbxImporter *pImporter = GetImporter();
  if (!pImporter->Initialize(path.c_str(), -1, m_pManager->GetIOSettings()))
    return nullptr;

  FbxScene *pScene = Create();
  if (!pImporter->Import(pScene) || pImporter->GetStatus().GetCode() != FbxStatus::eSuccess)
  {
    // TODO: Log error
    pScene->Destroy(true);
    return nullptr;
  }
  
  FbxSystemUnit::m.ConvertScene(pScene);

  return pScene;
}

bool atFBXCommon::Export(const atFilename &path, FbxScene *pScene)
{
  return false;
}

FbxScene* atFBXCommon::Create()
{
  return FbxScene::Create(m_pManager, "");
}

FbxManager* atFBXCommon::GetManager()
{
  return m_pManager;
}

FbxImporter* atFBXCommon::GetImporter()
{
  if (!m_pImporter)
    m_pImporter = FbxImporter::Create(m_pManager, "");
  return m_pImporter;
}

FbxExporter* atFBXCommon::GetExporter()
{
  if (!m_pExporter)
    m_pExporter = FbxExporter::Create(m_pManager, "");
  return m_pExporter;
}

template<> atMat4D &atAssign(atMat4D &lhs, const FbxAMatrix &rhs) { lhs = (*(atMat4D *)(&rhs)).Transpose(); return lhs; }
template<> FbxAMatrix &atAssign(FbxAMatrix &lhs, const atMat4D &rhs) { lhs = (*(FbxAMatrix *)&rhs).Transpose(); return lhs; }
