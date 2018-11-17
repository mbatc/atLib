#ifndef atFontRenderer_h__
#define atFontRenderer_h__

#include "atFilename.h"
#include "atWindow.h"

class atFontRenderer
{
public:
  atFontRenderer() = delete;
  static void SetFont(const atFilename &font);
  static void Bake(const int64_t x, const int64_t y, const atString &text);
  static void Draw(const atWindow &wnd);
};

#endif // atFontRenderer_h__
