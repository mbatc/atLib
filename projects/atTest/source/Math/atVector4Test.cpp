#include "atMathTest.h"

void tests::math::Vector4()
{
  atVec4I a(1);
  atRelAssert(a.x == 1 && a.y == 1 && a.z == 1 && a.w == 1, "atVector4::atVector4(val) failed.");

  atVec4I b(1, 2, 3, 4);
  atRelAssert(b.x == 1 && b.y == 2 && b.z == 3 && b.w == 4, "atVector4::atVector4(val) failed.");

  atVec4I c(atVec2I(1, 2), 3, 4);
  atRelAssert(c.x == 1 && c.y == 2 && c.z == 3 && c.w == 4, "atVector4::atVector4(atVec2I, z, w) failed.");

  atVec4I d(atVec2I(1, 2), atVec2I(3, 4));
  atRelAssert(d.x == 1 && d.y == 2 && d.z == 3 && d.w == 4, "atVector4::atVector4(atVec2I, atVec2I) failed.");

  atVec4I e(1, atVec2I(2, 3), 4);
  atRelAssert(e.x == 1 && e.y == 2 && e.z == 3 && e.w == 4, "atVector4::atVector4(x, atVec2I, w) failed.");

  atVec4I f(1, 2, atVec2I(3, 4));
  atRelAssert(f.x == 1 && f.y == 2 && f.z == 3 && f.w == 4, "atVector4::atVector4(x, y, atVec2I) failed.");

  atVec4I g(1, atVec3I(2, 3, 4));
  atRelAssert(g.x == 1 && g.y == 2 && g.z == 3 && g.w == 4, "atVector4::atVector4(x, atVec3I) failed.");

  atVec4I h(atVec3I(1, 2, 3), 4);
  atRelAssert(h.x == 1 && h.y == 2 && h.z == 3 && h.w == 4, "atVector4::atVector4(atVec3I, w) failed.");

  atRelAssert(atVec4I(1, 2, 3, 4) == atVec4I(1, 2, 3, 4), "atVector4::operator==(atVector4) failed.");
  atRelAssert(atVec4I(1, 2, 3, 4) != atVec4I(4, 3, 2, 1) &&
    atVec4I(1, 2, 3, 4) != atVec4I(4, 6, 7, 8) &&
    atVec4I(1, 2, 2, 2) != atVec4I(1, 3, 3, 3) &&
    atVec4I(1, 1, 2, 2) != atVec4I(1, 1, 3, 3) &&
    atVec4I(1, 1, 1, 2) != atVec4I(1, 1, 1, 3), "atVector4::operator!=(atVector4) failed.");

  atRelAssert(atVec4I(2, 3, 4, 5) + atVec4I(6, 7, 8, 9) == atVec4I(8, 10, 12, 14), "atVector4::operator+() failed.");
  atRelAssert(atVec4I(6, 7, 8, 9) - atVec4I(5, 4, 3, 2) == atVec4I(1, 3, 5, 7), "atVector4::operator-() failed.");
  atRelAssert(atVec4I(2, 3, 4, 5) * atVec4I(6, 7, 8, 9) == atVec4I(12, 21, 32, 45), "atVector4::operator*() failed.");
  atRelAssert(atVec4I(12, 21, 32, 45) / atVec4I(6, 7, 8, 9) == atVec4I(2, 3, 4, 5), "atVector4::operator/() failed.");
}
