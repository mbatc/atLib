#include "atMathTest.h"
#include "atContainerTest.h"

void atTest()
{
  tests::math::Functions();
  tests::math::Vector2();
  tests::math::Vector3();
  tests::math::Vector4();
  tests::math::Matrix();

  tests::containers::HashMap();
  tests::containers::Vector();
  tests::containers::String();
}