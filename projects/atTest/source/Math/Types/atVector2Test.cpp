#include "atMathTest.h"

void tests::math::Vector2()
{
  atVec2I a(1);
  atRelAssert(a.x == 1 && a.y == 1, "atVector2::atVector2(val) failed.");
  atVec2I b(1, 2);
  atRelAssert(b.x == 1 && b.y == 2, "atVector2::atVector2(val) failed.");
  atRelAssert(atVec2I(1, 1) == atVec2I(1, 1), "atVector2::operator==() failed.");
  atRelAssert(
    atVec2I(1, 1) != atVec2I(1, 2) &&
    atVec2I(1, 2) != atVec2I(3, 4) &&
    atVec2I(1, 2) != atVec2I(3, 2), "atVector2::operator!=() failed.");
  atRelAssert(atVec2I(5, 4) + atVec2I(3, 2) == atVec2I(8, 6), "atVector2::operator+() failed.");
  atRelAssert(atVec2I(5, 6) - atVec2I(3, 2) == atVec2I(2, 4), "atVector2::operator-() failed.");
  atRelAssert(atVec2I(5, 4) * atVec2I(3, 2) == atVec2I(15, 8), "atVector2::operator*() failed.");
  atRelAssert(atVec2I(6, 8) / atVec2I(3, 2) == atVec2I(2, 4), "atVector2::operator/() failed.");
}
