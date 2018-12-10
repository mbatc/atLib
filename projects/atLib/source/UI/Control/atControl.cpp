#include "atControl.h"
#include "atPrimitiveRenderer.h"
#include "atInput.h"
#include "atIntersects.h"

static atControlContext s_context;

atVec4I _GetItemRect(const atControlActiveRect &item);
void _UpdateItem(const atControlActiveRect &item);
void _AdvanceCursor(const bool sameLine = false);
atControlActiveRect& _GetItem(const int64_t id);

void atControl::BeginFrame()
{
  for (auto &kvp : s_context.m_items) kvp.m_val.added = false;
  s_context.activeID = AT_INVALID_ID;
  s_context.hoveredID = AT_INVALID_ID;
  s_context.lastID = AT_INVALID_ID;
}

bool atControl::Button(const atString &name)
{
  atControlActiveRect &item = _GetItem(atHash::Hash(name));
  item.position = s_context.cursor;
  if (item.added)
    item.size = atPrimitiveRenderer::TextSize(name) + 8;
  _UpdateItem(item);
  _AdvanceCursor();

  atVec4F bgColor = atVec4F(0.3f, 0.3f, 0.3f, .7f);
  if (IsItemHot())
    bgColor = atVec4F(0.2f, 0.2f, 0.2f, .7f);
  else if (IsItemHovered())
    bgColor = atVec4F(0.6f, 0.6f, 0.6f, .7f);

  atPrimitiveRenderer::PushColour(bgColor);
  atPrimitiveRenderer::AddRectangle(item.position, item.position + item.size);
  atPrimitiveRenderer::PopColour();

  Text(name, item.position + 4);
  return IsItemActive();
}

bool atControl::Selectable(const atString &name, bool *pSelected)
{
  bool active = Selectable(name, *pSelected);
  atControlActiveRect &item = _GetItem(atHash::Hash(name + "selectable"));
  if (IsItemActive()) *pSelected = !(*pSelected);
  return active;
}

bool atControl::Selectable(const atString &name, const bool selected)
{
  atControlActiveRect &item = _GetItem(atHash::Hash(name + "selectable"));
  item.position = s_context.cursor;
  if (item.added) item.size = atPrimitiveRenderer::TextSize(name) + 8;
  _UpdateItem(item);
  _AdvanceCursor();
  
  atVec4F bgColor = 0;
  if (IsItemHot())
    bgColor = atVec4F(0.2f, 0.2f, 0.2f, .7f);
  else if (IsItemHovered())
    bgColor = atVec4F(0.6f, 0.6f, 0.6f, .7f);
  else if (selected)
    bgColor = atVec4F(0.7f, 0.7f, 0.7f, .7f);

  atPrimitiveRenderer::PushColour(bgColor);
  atPrimitiveRenderer::AddRectangle(item.position, item.position + item.size);
  atPrimitiveRenderer::PopColour();
  Text(name, item.position + 4);
  return IsItemActive();
}

bool atControl::CollapsingHeader(const atString &name)
{
  atControlActiveRect &item = _GetItem(atHash::Hash(name + "collapsingHeader"));
  item.position = s_context.cursor;
  if (item.added)
  {
    item.size = atPrimitiveRenderer::TextSize(name) + 8;
    item.size.x += item.size.y + 5;
    item.data.AddMember("isOpen", false);
  }
  _UpdateItem(item);
  _AdvanceCursor();
  bool isOpen = item.data["isOpen"];

  atVec4F bgColor = atVec4F(0.3f, 0.3f, 0.3f, .7f);
  if (IsItemHot())
    bgColor = atVec4F(0.2f, 0.2f, 0.2f, .7f);
  else if (IsItemHovered())
    bgColor = atVec4F(0.6f, 0.6f, 0.6f, .7f);
  item.data["isOpen"] = IsItemActive() ? !isOpen : isOpen;

  atPrimitiveRenderer::PushColour(bgColor);
  atPrimitiveRenderer::AddRectangle(item.position, item.position + item.size);
  atPrimitiveRenderer::PopColour();
  atPrimitiveRenderer::AddCircle(item.position.x + item.size.y / 2, item.position.y + item.size.y / 2, 10, 3, isOpen ? atPi / 2.0 : 0.0, atVec2F(.5f, .5f));
  Text(name, item.position + atVec2I(item.size.y + 5, 0) + 4);
  return isOpen;
}

// bool atControl::BeginComboBox(const atString &name, const atString &preview)
// {
//   atControlActiveRect &item = _GetItem(atHash::Hash(name + "comboBoxHeader"));
//   item.position = s_context.cursor;
//   if (item.added)
//   {
//     item.size = atPrimitiveRenderer::TextSize(name) + 8;
//     item.size.x += item.size.y + 5;
//     item.data.AddMember("isOpen", false);
//   }
// 
//   _UpdateItem(item);
//   _AdvanceCursor();
// 
//   bool isOpen = item.data["isOpen"];
//   item.data["isOpen"] = IsItemActive() ? !isOpen : isOpen;
// 
//   atVec4F bgColor = atVec4F(0.3f, 0.3f, 0.3f, .7f);
//   if (IsItemHot())
//     bgColor = atVec4F(0.2f, 0.2f, 0.2f, .7f);
//   else if (IsItemHovered())
//     bgColor = atVec4F(0.6f, 0.6f, 0.6f, .7f);
// 
//   atPrimitiveRenderer::PushColour(bgColor);
//   atPrimitiveRenderer::AddRectangle(item.position, item.position + item.size);
//   atPrimitiveRenderer::PopColour();
//   Text(name, item.position + atVec2I(item.size.y + 5, 0));
//   return item.data["isOpen"];
// }
// 
// bool atControl::EndComboBox()
// {
//   return false;
// }

bool atControl::IsMouseHovering(const atVec4I &rect)
{
  const atVec2I &pos = atInput::MousePos();
  return atIntersects(rect, atVec4I(pos, pos));
}

void atControl::Text(const atString &text, const atVec2I &position) { atPrimitiveRenderer::AddText(position.x, position.y, text); }
bool atControl::IsItemHovered() { return s_context.hoveredID == s_context.lastID; }
void atControl::Draw(const atWindow &window) { atPrimitiveRenderer::Draw(window); }
bool atControl::IsItemActive() { return s_context.activeID == s_context.lastID; }
bool atControl::IsItemHot() { return s_context.hotID == s_context.lastID; }
void atControl::SetCursor(const atVec2I &pos) { s_context.cursor = pos; }
void atControl::EndFrame() {}

// Static Internals

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

void _UpdateItem(const atControlActiveRect &item)
{
  s_context.lastID = item.id;
  atVec4I rect = _GetItemRect(item);
  if (atControl::IsMouseHovering(rect))
  {
    s_context.hoveredID = item.id;
    if (atInput::MousePressed(atMB_Left))
      s_context.hotID = item.id;
  }

  if (atInput::MouseReleased(atMB_Left))
    if (s_context.hotID == item.id)
    {
      s_context.hotID = AT_INVALID_ID;
      s_context.activeID = atControl::IsMouseHovering(rect) ? item.id : AT_INVALID_ID;
    }

}

void _AdvanceCursor(const bool sameLine)
{
  atControlActiveRect& lastItem = _GetItem(s_context.lastID);
  s_context.cursor.x = lastItem.position.x + (lastItem.size.x + s_context.layout.itemSpacing.x) * sameLine;
  s_context.cursor.y = lastItem.position.y + (lastItem.size.y + s_context.layout.itemSpacing.y) * !sameLine;
}

atVec4I _GetItemRect(const atControlActiveRect &item) { return atVec4I(item.position, item.position + item.size); }