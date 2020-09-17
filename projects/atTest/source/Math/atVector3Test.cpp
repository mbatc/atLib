#include "atMathTest.h"

void tests::math::Vector3()
{
  atVec3I a(1);
  atRelAssert(a.x == 1 && a.y == 1 && a.z == 1, "atVector3::atVector3(val) failed.");
  atVec3I b(1, 2, 3);
  atRelAssert(b.x == 1 && b.y == 2 && b.z == 3, "atVector3::atVector3(val) failed.");
  atVec3I c(atVec2I(1, 2), 3);
  atRelAssert(c.x == 1 && c.y == 2 && c.z == 3, "atVector3::atVector3(atVec2I, z) failed.");
  atVec3I d(1, atVec2I(2, 3));
  atRelAssert(d.x == 1 && d.y == 2 && d.z == 3, "atVector3::atVector3(x, atVec2I) failed.");
  atRelAssert(atVec3I(1, 2, 3) == atVec3I(1, 2, 3), "atVector3::operator==(atVector3) failed.");
  atRelAssert(atVec3I(1, 2, 3) != atVec3I(3, 2, 1) &&
    atVec3I(1, 2, 3) != atVec3I(4, 5, 6) &&
    atVec3I(1, 2, 2) != atVec3I(1, 3, 3) &&
    atVec3I(1, 2, 3) != atVec3I(2, 2, 3), "atVector3::operator!=(atVector3) failed.");

  atRelAssert(atVec3I(2, 3, 4) + atVec3I(5, 6, 7) == atVec3I(7, 9, 11), "atVector3::operator+() failed.");
  atRelAssert(atVec3I(2, 3, 4) * atVec3I(5, 6, 7) == atVec3I(10, 18, 28), "atVector3::operator*() failed.");
  atRelAssert(atVec3I(5, 6, 7) - atVec3I(4, 3, 2) == atVec3I(1, 3, 5), "atVector3::operator-() failed.");
  atRelAssert(atVec3I(10, 18, 28) / atVec3I(2, 3, 4) == atVec3I(5, 6, 7), "atVector3::operator/() failed.");
}
