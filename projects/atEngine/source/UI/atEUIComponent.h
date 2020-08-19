#ifndef atEUIComponent_h__
#define atEUIComponent_h__

#include <typeindex>

#include "atRect.h"
#include "atString.h"
#include "atKeyValue.h"
#include "atButtonState.h"
#include "atCanvas2D.h"

#include "atEUIStyle.h"

enum atEUIUnit
{
  atEUI_Pixels,     // Size on pixels
  atEUI_Percent,    // Percentage of the parent container dimension
  atEUI_Characters, // Size in characters
  atEUI_Count
};

class atEUIComponent
{
public:
  atEUIComponent();
  ~atEUIComponent();

  virtual void UpdateLayout();
  virtual void ProcessInput();
  virtual bool Draw(atCanvas2D *pGraphics);
  virtual bool DrawChildren(atCanvas2D *pGraphics);

  void SetPosition(const atVec2F &pos, const atEUIUnit &unit = atEUI_Pixels);
  void SetSize(const atVec2F &size, const atEUIUnit &unit = atEUI_Pixels);
  void SetScreenPosition(const atVec2F &pos, const atEUIUnit &unit = atEUI_Pixels);

  atVec2F Size() const;
  atVec2F ScreenPosition() const;
  atRectF ScreenRect() const;
  atVec2F LocalPosition() const;
  atRectF LocalRect() const;

  // Get the number of children
  int64_t ChildCount() const;

  // Get the child component at the specified index
  atEUIComponent* GetChild(const int64_t &idx) const;

  // Get the child component with the specified name
  atEUIComponent* GetChild(const atString &name) const;

  // Find a child component using a path relative to this component.
  //
  // A path contains component names separated by a forward or back slash.
  //   e.g. "root/container/myinput" or "root\\container\\myinput"
  //
  // Similar to file paths, ".." is a step up and "." is this component.
  //   e.g. "../a child of my parent" or "./one of my children"
  atEUIComponent* FindComponent(const atString &name) const;

  // Remove a child component
  bool RemoveChild(const int64_t &idx, const bool &deleteComponent = true);
  bool RemoveChild(const atString &name, const bool &deleteComponent = true);
  bool RemoveChild(atEUIComponent *pComponent, const bool &deleteComponent = true);

  // Add an existing component as a child - this will remove it from an existing parent
  atEUIComponent* AddChild(const atString &name, atEUIComponent *pComponent);

  // Create and add a child
  template<typename T, typename... Args> T* AddChild(const atString &name, Args&&... args);

  // Create a UI component
  template<typename T, typename... Args> static atEUIComponent* Create(Args&&... args);

  // Get the root component
  atEUIComponent* GetRoot() const;

  // Get the parent component
  atEUIComponent* GetParent() const;

  // Check if this is the root component of the UI
  bool IsRoot() const;

  // Is this component hovered
  bool IsHovered(const bool &includeChildren = false) const;

  // Is any child component hovered
  bool IsChildHovered() const;

  // Is any child down
  bool IsChildActive() const;

  // Mouse is pressed while hovering the component
  bool IsPressed() const;

  // Mouse is released while hovering the component
  // after pressing the component
  bool IsActive() const;

  // Mouse is released after pressing the component
  bool IsReleased() const;

  // Mouse is held after pressing the component
  bool IsDown() const;

  // Mouse double clicked the component
  bool IsDoubleClicked() const;

  // Number of consecutive double clicks
  int64_t DoubleClickCount() const;

  // Set the style used by this component
  void SetStyle(atEUIStyle *pStyle);

  // Get the style used by this component
  atEUIStyle* GetStyle() const;

  // Event callbacks
  void OnPressed(std::function<void(atEUIComponent*)> callback);
  void OnDown(std::function<void(atEUIComponent *)> callback);
  void OnActive(std::function<void(atEUIComponent *)> callback);
  void OnReleased(std::function<void(atEUIComponent *)> callback);
  void OnDoubleClicked(std::function<void(atEUIComponent *)> callback);
  void OnHovered(std::function<void(atEUIComponent *)> callback);
  void OnChildHovered(std::function<void(atEUIComponent *)> callback);
  void OnChildActive(std::function<void(atEUIComponent *)> callback);

protected:
  virtual void OnAddChild(atEUIComponent *pChild);

  bool DrawBorder(atCanvas2D *pGraphics) const;

  atEUIComponent* FindComponent(const atVector<atString> &path, const int64_t &currentIdx) const;

  int64_t GetChildIndex(const atEUIComponent *pComponent) const;
  int64_t GetChildIndex(const atString &name) const;

  struct InteractionState
  {
    bool active;
    bool hovered;
    bool childHovered;
    bool childActive;

    atButtonState action;
  } m_state;

private:
  std::type_index m_typeInfo;

  atVec2F m_size;
  atVec2F m_position;

  atRectF m_rect;

  int64_t m_componentID = 0;

  atEUIStyle *m_pStyle = atEUIStyle::GlobalStyle();
  atEUIComponent *m_pRoot = nullptr;   // Root component of the UI
  atEUIComponent *m_pParent = nullptr; // This components parent

  atVector<atKeyValue<atString, atEUIComponent*>> m_children;

  // Event callbacks
  std::function<void(atEUIComponent *)> m_onPressed;
  std::function<void(atEUIComponent *)> m_onDown;
  std::function<void(atEUIComponent *)> m_onActive;
  std::function<void(atEUIComponent *)> m_onReleased;
  std::function<void(atEUIComponent *)> m_onDoubleClicked;
  std::function<void(atEUIComponent *)> m_onHovered;
  std::function<void(atEUIComponent *)> m_onChildHovered;
  std::function<void(atEUIComponent *)> m_onChildActive;

  static int64_t GetComponentID();
};

#include "atEUIComponent.inl"

#endif // atEUIComponent_h__
