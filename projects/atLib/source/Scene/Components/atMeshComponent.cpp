#include "atSceneComponent.h"
#include "atMeshComponent.h"
#include "atSceneNode.h"
#include "atScene.h"

atMeshComponent::atMeshComponent(const atString &name) : atSceneComponent(TypeID(), name) {}

bool atMeshComponent::Init()
{
  m_renderable.SetMaterial(m_material);
  return true;
}

bool atMeshComponent::Update(const double &deltaTime) { atUnused(deltaTime); return true; }

bool atMeshComponent::Render(const atMat4D &vp)
{
  atTransformComponent *pTransform = GetNode()->GetTransform();
  atMat4F modelMat = pTransform->GlobalTransformMat();
  atMat4F mvp = atMat4F(vp * modelMat);
  atMat4F normMat = modelMat.Inverse();
  m_renderable.SetUniform("modelMat", modelMat.Transpose());
  m_renderable.SetUniform("normalMat", normMat.Transpose());
  m_renderable.SetUniform("mvp", mvp.Transpose());
  return m_renderable.Draw(false);
}

bool atMeshComponent::Destroy() { return true; }

atSceneComponent* atMeshComponent::Copy() const
{
  atMeshComponent *pNew = atNew<atMeshComponent>(m_name);
  pNew->m_material = m_material;
  pNew->m_renderable = m_renderable;
  return pNew;
}

atCoreComponentType atMeshComponent::TypeID() { return atCCT_Mesh; }
