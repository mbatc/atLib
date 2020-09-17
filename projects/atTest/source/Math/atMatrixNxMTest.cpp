#include "atMathTest.h"

void tests::math::MatrixNxM()
{
  // Test Indexing
  {
    atMatrixNxM<int64_t> testMat(4, 4);
    testMat.m_data = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
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


    atRelAssert(testMat(0, 0) == 1, "m00 is incorrect.");
    atRelAssert(testMat(0, 1) == 2, "m01 is incorrect.");
    atRelAssert(testMat(0, 2) == 3, "m02 is incorrect.");
    atRelAssert(testMat(0, 3) == 4, "m03 is incorrect.");

    atRelAssert(testMat(1, 0) == 5, "m10 is incorrect.");
    atRelAssert(testMat(1, 1) == 6, "m11 is incorrect.");
    atRelAssert(testMat(1, 2) == 7, "m12 is incorrect.");
    atRelAssert(testMat(1, 3) == 8, "m13 is incorrect.");

    atRelAssert(testMat(2, 0) == 9, "m20 is incorrect.");
    atRelAssert(testMat(2, 1) == 10, "m21 is incorrect.");
    atRelAssert(testMat(2, 2) == 11, "m22 is incorrect.");
    atRelAssert(testMat(2, 3) == 12, "m23 is incorrect.");

    atRelAssert(testMat(3, 0) == 13, "m30 is incorrect.");
    atRelAssert(testMat(3, 1) == 14, "m31 is incorrect.");
    atRelAssert(testMat(3, 2) == 15, "m32 is incorrect.");
    atRelAssert(testMat(3, 3) == 16, "m33 is incorrect.");
  }

  { // Transpose test
    atMatrixNxM<float> testMat(4, 4);
    testMat.m_data = {
      1, 2, 3, 4,
      5, 6, 7, 8,
      9, 10, 11, 12,
      13, 14, 15, 16
    };

    atMatrixNxM<float> transposed(4, 4);
    transposed.m_data = {
      1, 5, 9, 13,
      2, 6, 10, 14,
      3, 7, 11, 15,
      4, 8, 12, 16
    };

    atRelAssert(testMat.Transpose() == transposed, "atMatrix::Transpose() is incorrect.");
  }

  { // Inverse/Determinate tests
    int det2x2 = atMatrixDet2x2(3, 8, 4, 6);
    atRelAssert(det2x2 == -14, "atMatrixDet2x2 is incorrect.");

    int det3x3 = atMatrixDet3x3(6, 1, 1, 4, -2, 5, 2, 8, 7);
    atRelAssert(det3x3 == -306, "atMatrixDet3x3 is incorrect.");

    atMatrixNxM<float> testMat(4, 4);
    testMat.m_data =
    { 1, 1, 1, -1,
      1, 1, -1, 1,
      1, -1, 1, 1,
      -1, 1, 1, 1
    };

    atRelAssert(testMat.Cofactors().m_data == atVector<float>({
      -4, -4, -4, 4,
        -4, -4, 4, -4,
        -4, 4, -4, -4,
        4, -4, -4, -4 }), "Matrix determinate failed!");

    atRelAssert(testMat.Determinate() == -16, "Matrix determinate failed!");

    atRelAssert(testMat.Inverse().m_data == atVector<float>({
      0.25, 0.25, 0.25, -0.25,
      0.25, 0.25, -0.25, 0.25,
      0.25, -0.25, 0.25, 0.25,
      -0.25, 0.25, 0.25, 0.25 }), "Matrix Inverse Failed!");
  }
}