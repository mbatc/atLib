#include "atMathTest.h"

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