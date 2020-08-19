#include "atEUIComponent.h"
#include "atInput.h"

atEUIComponent::atEUIComponent()
  : m_componentID(GetComponentID())
  , m_typeInfo(typeid(void))
  , m_pRoot(this)
  , m_pParent(nullptr)
{}

atEUIComponent::~atEUIComponent()
{
  while (ChildCount() > 0)
    RemoveChild(ChildCount() - 1);
}

void atEUIComponent::UpdateLayout()
{
  // Update the child layout
  for (const atKeyValue<atString, atEUIComponent*> &kvp : m_children)
    kvp.m_val->UpdateLayout();
}

bool atEUIComponent::Draw(atCanvas2D *pGraphics) { return true; }

bool atEUIComponent::DrawChildren(atCanvas2D *pGraphics)
{
  for (const atKeyValue<atString, atEUIComponent*> &kvp : m_children)
    if (kvp.m_val->Draw(pGraphics))
      kvp.m_val->DrawChildren(pGraphics);
  return true;
}

void atEUIComponent::SetPosition(const atVec2F &pos, const atEUIUnit &unit) { m_position = pos; }
void atEUIComponent::SetSize(const atVec2F &size, const atEUIUnit &unit) { m_size = size; }

void atEUIComponent::SetScreenPosition(const atVec2F &pos, const atEUIUnit &unit)
{
  atVec2F parentPos = 0;
  if (GetParent())
    parentPos = GetParent()->ScreenPosition();
  SetPosition(pos - parentPos);
}

atVec2F atEUIComponent::Size() const { return m_size; }
atVec2F atEUIComponent::ScreenPosition() const { return GetParent() ? GetParent()->ScreenPosition() + m_position : m_position; }

atRectF atEUIComponent::ScreenRect() const
{
  atVec2F pos = ScreenPosition();
  return atRectF(pos, pos + Size());
}

atVec2F atEUIComponent::LocalPosition() const { return m_position; }
atRectF atEUIComponent::LocalRect() const { return atRectF(LocalPosition(), LocalPosition() + Size()); }

void atEUIComponent::ProcessInput()
{
  // Get input data
  atVec2F msPos = atInput::MousePos();
  bool msDown = atInput::LeftMouseDown();
  bool msPressed = atInput::LeftMousePressed();
  double dt = atInput::DeltaTime();

  // Reset some states
  m_state.childHovered = false;
  m_state.childActive = false;
  m_state.hovered = false;

  // Update child components and hover events
  for (const atKeyValue<atString, atEUIComponent*> &kvp : m_children)
  {
    atEUIComponent *pChild = kvp.m_val;
    pChild->ProcessInput();
    m_state.childHovered |= pChild->IsHovered();
    m_state.childActive |= pChild->IsDown() || pChild->IsChildActive();
  }
  
  // Update hovered state if none of the components children are hovered
  if (!m_state.childHovered)
    m_state.hovered = ScreenRect().Contains(msPos);

  if (!m_state.childActive)
  { // Don't process state changes if a child is active
    // Only continue holding down if it is already down
    if (IsDown() && msDown)
      m_state.action.OnDown(dt);

    // Only start being down if the component is hovered
    if (m_state.hovered && msPressed)
      m_state.action.OnDown(dt);

    // If the left mouse is not down do OnUp logic
    if (!msDown)
    {
      m_state.active = m_state.hovered && IsDown();
      m_state.action.OnUp(dt);
    }
  }

  if (IsPressed() && m_onPressed) m_onPressed(this);
  if (IsReleased() && m_onReleased) m_onReleased(this);
  if (IsDown() && m_onDown) m_onDown(this);
  if (IsActive() && m_onActive) m_onActive(this);
  if (IsDoubleClicked() && m_onDoubleClicked) m_onDoubleClicked(this);
  if (IsHovered() && m_onHovered) m_onHovered(this);
  if (IsChildActive() && m_onChildActive) m_onChildActive(this);
  if (IsChildHovered() && m_onChildHovered) m_onChildHovered(this);
}

int64_t atEUIComponent::ChildCount() const { return m_children.size(); }

atEUIComponent* atEUIComponent::GetChild(const int64_t &idx) const { return idx >= 0 && idx < m_children.size() ? m_children[idx].m_val : nullptr; }
atEUIComponent* atEUIComponent::GetChild(const atString &name) const { return GetChild(GetChildIndex(name)); }
atEUIComponent* atEUIComponent::FindComponent(const atString &name) const
{
  atVector<atString> path = name.split("/\\", true);
  return FindComponent(path, 0);
}

bool atEUIComponent::RemoveChild(const int64_t &idx, const bool &deleteComponent)
{
  atEUIComponent *pChild = GetChild(idx);
  if (!pChild)
    return false;

  m_children.erase(idx);
  atDelete(pChild);
  return true;
}

bool atEUIComponent::RemoveChild(const atString &name, const bool &deleteComponent) { return RemoveChild(GetChildIndex(name), deleteComponent); }
bool atEUIComponent::RemoveChild(atEUIComponent *pComponent, const bool &deleteComponent) { return RemoveChild(GetChildIndex(pComponent), deleteComponent); }

atEUIComponent* atEUIComponent::AddChild(const atString &name, atEUIComponent *pComponent)
{
  if (GetChild(name))
    return nullptr;

  atEUIComponent *pPrevParent = pComponent->GetParent();
  if (pPrevParent)
    pPrevParent->RemoveChild(pComponent, false);

  m_children.emplace_back(name, pComponent);
  pComponent->m_pRoot = m_pRoot;
  pComponent->m_pParent = this;
  return pComponent;
}

atEUIComponent* atEUIComponent::GetRoot() const { return m_pRoot; }
atEUIComponent* atEUIComponent::GetParent() const { return m_pParent; }
bool atEUIComponent::IsRoot() const { return m_pRoot == this; }

bool atEUIComponent::IsHovered(const bool &includeChildren) const { return m_state.hovered || (includeChildren && IsChildHovered()); }
bool atEUIComponent::IsChildHovered() const { return m_state.childHovered; }
bool atEUIComponent::IsChildActive() const { return m_state.childActive; }
bool atEUIComponent::IsPressed() const { return m_state.action.IsPressed(); }
bool atEUIComponent::IsActive() const { return m_state.active; }
bool atEUIComponent::IsReleased() const { return m_state.action.IsReleased(); }
bool atEUIComponent::IsDown() const { return m_state.action.IsDown(); }
bool atEUIComponent::IsDoubleClicked() const { return m_state.action.IsDoubleClicked(); }
int64_t atEUIComponent::DoubleClickCount() const { return m_state.action.DoubleClickCount(); }

void atEUIComponent::SetStyle(atEUIStyle *pStyle) { m_pStyle = pStyle; }
atEUIStyle* atEUIComponent::GetStyle() const { return m_pStyle; }

void atEUIComponent::OnPressed(std::function<void(atEUIComponent *)> callback) { m_onPressed = callback; }
void atEUIComponent::OnDown(std::function<void(atEUIComponent *)> callback) { m_onDown = callback; }
void atEUIComponent::OnActive(std::function<void(atEUIComponent *)> callback) { m_onActive = callback; }
void atEUIComponent::OnReleased(std::function<void(atEUIComponent *)> callback) { m_onReleased = callback; }
void atEUIComponent::OnDoubleClicked(std::function<void(atEUIComponent *)> callback) { m_onDoubleClicked = callback; }
void atEUIComponent::OnHovered(std::function<void(atEUIComponent *)> callback) { m_onHovered = callback; }
void atEUIComponent::OnChildHovered(std::function<void(atEUIComponent *)> callback) { m_onChildHovered = callback; }
void atEUIComponent::OnChildActive(std::function<void(atEUIComponent *)> callback) { m_onChildActive = callback; }
void atEUIComponent::OnAddChild(atEUIComponent *pChild) {}

bool atEUIComponent::DrawBorder(atCanvas2D *pGraphics) const
{
  atRectF rect = ScreenRect();
  atEUIStyle *pStyle = GetStyle();
  pGraphics->AddRect(rect.m_min, rect.m_max, pStyle->GetColour(atEUIStyle::Color_Background));
  return true;
}

atEUIComponent* atEUIComponent::FindComponent(const atVector<atString> &path, const int64_t &currentIdx) const
{
  const atString &childName = path[currentIdx];
  atEUIComponent *pFound = nullptr;

  if (childName == "..")
  {
    pFound = m_pParent;
  }
  else if (childName == ".")
  {
    pFound = (atEUIComponent*)this;
  }
  else
  {
    for (const atKeyValue<atString, atEUIComponent*> &kvp : m_children)
      if (kvp.m_key == childName)
      {
        pFound = kvp.m_val;
        break;
      }
  }

  if (currentIdx >= path.size() - 1) // At the end of the path so return the found component
    return pFound;
  return pFound ? pFound->FindComponent(path, currentIdx + 1) : nullptr;
}

int64_t atEUIComponent::GetChildIndex(const atEUIComponent *pComponent) const
{
  for (int64_t i = 0; i < m_children.size(); ++i)
    if (m_children[i].m_val == pComponent)
      return i;
  return -1;
}

int64_t atEUIComponent::GetChildIndex(const atString &name) const
{
  for (int64_t i = 0; i < m_children.size(); ++i)
    if (m_children[i].m_key == name)
      return i;
  return -1;
}

int64_t atEUIComponent::GetComponentID()
{
  static int64_t nextID = 0;
  return nextID;
}
