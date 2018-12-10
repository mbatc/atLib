#ifndef atControl_h__
#define atControl_h__

#include "atWindow.h"
#include "atHashMap.h"
#include "atObject.h"

struct atControlActiveRect
{
  atVec2I position;
  atVec2I size;
  int64_t id;
  int64_t zPos;

  atObject data;

  bool added;
};

struct atControlLayout
{
  atVec2I itemSpacing = { 10, 10 };
};

class atControlContext
{
public:
  atVec2I cursor;

  int64_t hotID = AT_INVALID_ID;
  int64_t activeID = AT_INVALID_ID;
  int64_t hoveredID = AT_INVALID_ID;
  int64_t lastID = AT_INVALID_ID;

  atControlLayout layout;

  atHashMap<int64_t, atControlActiveRect> m_items;
};

class atControl
{
public:
  atControl() = delete;

  static void BeginFrame();
  static void EndFrame();

  // static bool BeginComboBox(const atString &name, const atString &preview);
  // static bool EndComboBox();

  static bool Button(const atString &name);
  static bool Selectable(const atString &name, bool *pSelected = nullptr);
  static bool Selectable(const atString &name, const bool selected);
  static bool CollapsingHeader(const atString &name);
  static void Text(const atString &text, const atVec2I &position);

  static bool IsItemHot();
  static bool IsItemActive();
  static bool IsItemHovered();
  static bool IsMouseHovering(const atVec4I &rect);

  static void Draw(const atWindow &window);

  static void SetCursor(const atVec2I &pos);
};

#endif // atControl_h__
