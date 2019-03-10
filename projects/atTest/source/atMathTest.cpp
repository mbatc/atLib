#include "atMathTest.h"

void tests::math::Functions()
{
  // atMin(const T &a);
  // atMax(const T &a);
  // atMin(const T &first, const T2 &second, Args ...args);
  // atMax(const T &first, const T2 &second, Args ...args);
  // atSquare(const T x);
  // atSin(const T rads);
  // atCos(const T rads);
  // atTan(const T rads);
  // atASin(const T rads);
  // atACos(const T rads);
  // atATan(const T rads);
  // atATan2(const atVector2<T> &pos);
  // atATan2(const T y, const T x);
  // atQuadraticSolve(const T a, const T b, const T c);
  // atMod(const T a, const T b);
  // atMod(const float a, const float b);
  // atMod(const double a, const double b);
}

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

  atRelAssert(atVec3I(2, 3, 4) + atVec3I(5, 6, 7) == atVec3I(8, 9, 11), "atVector3::operator+() failed.");
  atRelAssert(atVec3I(5, 6, 7) - atVec3I(4, 3, 2) == atVec3I(1, 3, 5), "atVector3::operator-() failed.");
  atRelAssert(atVec3I(2, 3, 4) * atVec3I(5, 6, 7) == atVec3I(10, 18, 28), "atVector3::operator*() failed.");
  atRelAssert(atVec3I(10, 18, 28) / atVec3I(2, 3, 4) == atVec3I(5, 6, 7), "atVector3::operator/() failed.");
}

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

void tests::math::Matrix()
{
  atMat4F testMat(
    1, 2, 3, 4,
    5, 6, 7, 8,
    9, 10, 11, 12,
    13, 14, 15, 16);

  atRelAssert(testMat[0] == 1, "m00 is incorrect.");
  atRelAssert(testMat[1] == 2, "m01 is incorrect.");
  atRelAssert(testMat[2] == 3, "m02 is incorrect.");
  atRelAssert(testMat[3] == 4, "m03 is incorrect.");

  atRelAssert(testMat[4] == 5, "m10 is incorrect.");
  atRelAssert(testMat[5] == 6, "m11 is incorrect.");
  atRelAssert(testMat[6] == 7, "m12 is incorrect.");
  atRelAssert(testMat[7] == 8, "m13 is incorrect.");

  atRelAssert(testMat[8] == 9, "m20 is incorrect.");
  atRelAssert(testMat[9] == 10, "m21 is incorrect.");
  atRelAssert(testMat[10] == 11, "m22 is incorrect.");
  atRelAssert(testMat[11] == 12, "m23 is incorrect.");

  atRelAssert(testMat[12] == 13, "m30 is incorrect.");
  atRelAssert(testMat[13] == 14, "m31 is incorrect.");
  atRelAssert(testMat[14] == 15, "m32 is incorrect.");
  atRelAssert(testMat[15] == 16, "m33 is incorrect.");

  { // Transpose test
    atMat4F testMat(
      1, 2, 3, 4,
      5, 6, 7, 8,
      9, 10, 11, 12,
      13, 14, 15, 16
    );

    atRelAssert(testMat.Transpose() == atMat4F(
      1, 5, 9, 13,
      2, 6, 10, 14,
      3, 7, 11, 15,
      4, 8, 12, 16), "atMatrix::Transpose() is incorrect.");
  }

  { // Inverse/Determinate tests
    int det2x2 = atMatrixDet2x2(3, 8, 4, 6);
    atRelAssert(det2x2 == -14, "atMatrixDet2x2 is incorrect.");

    int det3x3 = atMatrixDet3x3(6, 1, 1, 4, -2, 5, 2, 8, 7);
    atRelAssert(det3x3 == -306, "atMatrixDet3x3 is incorrect.");

    atMat4F testMat = atMat4F(
      1, 1, 1, -1,
      1, 1, -1, 1,
      1, -1, 1, 1,
      -1, 1, 1, 1);

    atRelAssert(testMat.Cofactors() == atMat4F(
      -4, -4, -4, 4,
      -4, -4, 4, -4,
      -4, 4, -4, -4,
      4, -4, -4, -4), "Matrix determinate failed!");

    atRelAssert(testMat.Determinate() == -16, "Matrix determinate failed!");

    atRelAssert(testMat.Inverse() == atMat4F(
      0.25, 0.25, 0.25, -0.25,
      0.25, 0.25, -0.25, 0.25,
      0.25, -0.25, 0.25, 0.25,
      -0.25, 0.25, 0.25, 0.25), "Matrix Inverse Failed!");
  }
}