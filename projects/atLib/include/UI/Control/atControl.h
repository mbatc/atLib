#ifndef atControl_h__
#define atControl_h__

#include "atWindow.h"
#include "atHashMap.h"

struct atControlActiveRect
{
  atVec2I position;
  atVec2I size;
  int64_t id;
  int64_t zPos;
  bool added;
};

class atControlContext
{
public:
  int64_t hotID = AT_INVALID_ID;
  int64_t activeID = AT_INVALID_ID;
  int64_t hoveredID = AT_INVALID_ID;
  int64_t lastID = AT_INVALID_ID;
  atHashMap<int64_t, atControlActiveRect> m_items;
};

class atControl
{
public:
  atControl() = delete;

  static void BeginFrame();
  static void EndFrame();

  static bool Button(const atString &name, const atVec2I &position);
  static void Text(const atString &text, const atVec2I &position);

  static bool IsItemHot();
  static bool IsItemActive();
  static bool IsItemHovered();

  static bool IsMouseHovering(const atVec4I &rect);

  static void Draw(const atWindow &window);
};

#endif // atControl_h__
