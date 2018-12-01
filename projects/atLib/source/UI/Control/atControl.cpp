#include "atControl.h"
#include "atPrimitiveRenderer.h"
#include "atInput.h"
#include "atIntersects.h"

static atControlContext s_context;

atControlActiveRect& _GetItem(const int64_t id);
atVec4I _GetItemRect(const atControlActiveRect &item);

void atControl::BeginFrame()
{
  s_context.activeID = AT_INVALID_ID;
  s_context.hoveredID = AT_INVALID_ID;
  s_context.lastID = AT_INVALID_ID;
}

void atControl::EndFrame() {}

bool atControl::Button(const atString &name, const atVec2I &position)
{
  atControlActiveRect &item = _GetItem(atHash::Hash(name));
  s_context.lastID = item.id;

  item.position = position;
  if (item.added)
  {
    item.size = atPrimitiveRenderer::TextSize(name) + 8;
    item.added = false;
  }

  atVec4I rect = _GetItemRect(item);
  if (IsMouseHovering(rect))
  {
    s_context.hoveredID = item.id;
    if (atInput::MousePressed(atMB_Left))
      s_context.hotID = item.id;
  }

  if (atInput::MouseReleased(atMB_Left))
    if (s_context.hotID == item.id)
    {
      s_context.hotID = AT_INVALID_ID;
      s_context.activeID = IsMouseHovering(rect) ? item.id : AT_INVALID_ID;
    }
  atVec4F bgColor = atVec4F(0.3f, 0.3f, 0.3f, .7f);
  if (IsItemHot())
    bgColor = atVec4F(0.2f, 0.2f, 0.2f, .7f);
  else if (IsItemHovered())
    bgColor = atVec4F(0.6f, 0.6f, 0.6f, .7f);

  atPrimitiveRenderer::PushColour(bgColor);
  atPrimitiveRenderer::AddRectangle(item.position, item.position + item.size);
  atPrimitiveRenderer::PopColour();

  Text(name, position + 4);
  return IsItemActive();
}

bool atControl::IsMouseHovering(const atVec4I &rect)
{
  const atVec2I &pos = atInput::MousePos();
  return atIntersects(rect, atVec4I(pos, pos));
}

void atControl::Text(const atString &text, const atVec2I &position) { atPrimitiveRenderer::AddText(position.x, position.y, text); }
bool atControl::IsItemHot() { return s_context.hotID == s_context.lastID; }
bool atControl::IsItemActive() { return s_context.activeID == s_context.lastID; }
bool atControl::IsItemHovered() { return s_context.hoveredID == s_context.lastID; }
void atControl::Draw(const atWindow &window) { atPrimitiveRenderer::Draw(window); }

atControlActiveRect& _GetItem(const int64_t id)
{
  atControlActiveRect *pItem = s_context.m_items.TryGet(id);
  if (pItem) return *pItem;
  s_context.m_items.Add(id);
  atControlActiveRect &item = s_context.m_items[id];
  item.id = id;
  item.zPos = 0;
  item.added = true;
  return item;
}

atVec4I _GetItemRect(const atControlActiveRect &item) { return atVec4I(item.position, item.position + item.size); }