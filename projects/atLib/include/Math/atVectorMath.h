
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#ifndef atVectorMath_h__
#define atVectorMath_h__

#include <minmax.h>
#include <math.h>

class atVectorMath
{
public:
  atVectorMath() = delete;
  //------------------
  // Scalar Operations
  //

  template <typename VecType, template <typename> class VecCls, typename ValType> static VecCls<VecType> Assign(VecCls<VecType> *pLhs, const ValType val) { for (VecType &mine : *pLhs) mine = (VecType)val; return *pLhs; }
  template <typename VecType, template <typename> class VecCls, typename ValType> static bool Equals(const VecCls<VecType> &lhs, const ValType val) { for (const VecType val : lhs) if (val != (VecType)val) return false; return true; }
  
  template <typename VecType, template <typename> class VecCls, typename ValType> static VecCls<VecType> Add(const VecCls<VecType> &lhs, const ValType val) 
  { 
    VecCls<VecType> ret;
    for (int64_t i = 0; i < ret.element_count; ++i)
      ret[i] = lhs[i] + (VecType)val;
    return ret;
  }

  template <typename VecType, template <typename> class VecCls, typename ValType> static VecCls<VecType> Subtract(const VecCls<VecType> &lhs, const ValType val)
  {
    VecCls<VecType> ret;
    for (int64_t i = 0; i < ret.element_count; ++i)
      ret[i] = lhs[i] - (VecType)val;
    return ret;
  }  

  template <typename VecType, template <typename> class VecCls, typename ValType> static VecCls<VecType> Divide(const VecCls<VecType> &lhs, const ValType val)
  {
    VecCls<VecType> ret;
    for (int64_t i = 0; i < ret.element_count; ++i)
      ret[i] = lhs[i] / (VecType)val;
    return ret;
  }

  template <typename VecType, template <typename> class VecCls, typename ValType> static VecCls<VecType> Multiply(const VecCls<VecType> &lhs, const ValType val)
  {
    VecCls<VecType> ret;
    for (int64_t i = 0; i < ret.element_count; ++i)
      ret[i] = lhs[i] * (VecType)val;
    return ret;
  }  

  //------------------
  // Vector Operations
  //
  template <template <typename> class VecCls, typename VecType> static VecCls<VecType> Add(const VecCls<VecType> &lhs, const VecCls<VecType> &rhs) { return Add<VecCls, VecType, VecCls, VecType>(lhs, rhs); }
  template <template <typename> class VecCls, typename VecType> static VecCls<VecType> Subtract(const VecCls<VecType> &lhs, const VecCls<VecType> &rhs) { return Subtract<VecCls, VecType, VecCls, VecType>(lhs, rhs); }
  template <template <typename> class VecCls, typename VecType> static VecCls<VecType> Divide(const VecCls<VecType> &lhs, const VecCls<VecType> &rhs) { return Divide<VecCls, VecType, VecCls, VecType>(lhs, rhs); }
  template <template <typename> class VecCls, typename VecType> static VecCls<VecType> Multiply(const VecCls<VecType> &lhs, const VecCls<VecType> &rhs) { return Multiply<VecCls, VecType, VecCls, VecType>(lhs, rhs); }
  template <template <typename> class VecCls, typename VecType> static VecCls<VecType> Cross(const VecCls<VecType> &lhs, const VecCls<VecType> &rhs) { return Cross<VecCls, VecType, VecCls, VecType>(lhs, rhs); }
  template <template <typename> class VecCls, typename VecType> static VecType Dot(const VecCls<VecType> &lhs, const VecCls<VecType> &rhs) { return Dot<VecCls, VecType, VecCls, VecType>(lhs, rhs); }
  template <template <typename> class VecCls, typename VecType> static bool Equals(const VecCls<VecType> &lhs, const VecCls<VecType> &rhs) { return Equals<VecCls, VecType, VecCls, VecType>(lhs, rhs); }
  template <template <typename> class VecCls, typename VecType> static VecCls<VecType> Assign(VecCls<VecType> *pLhs, const VecCls<VecType> &rhs) { memcpy(pLhs, &rhs, sizeof(VecCls<VecType>)); return *pLhs; }
  template <template <typename> class VecCls, typename VecType> static VecType Angle(const VecCls<VecType> &lhs, const VecCls<VecType> &rhs) { return Angle<VecCls, VecType, VecCls, VecType>(lhs, rhs); }
  template <template <typename> class VecCls, typename VecType, typename VecType2> static VecCls<VecType> Reflect(const VecCls<VecType> &dir, const VecCls<VecType2> &norm) { return Subtract(dir, Multiply(norm, Dot(dir, norm) * 2)); }
  template <template <typename> class VecCls, typename VecType> static VecCls<VecType> Reflect(const VecCls<VecType> &dir, const VecCls<VecType> &norm) { return Reflect<VecCls, VecType, VecType>(dir, norm); }

  template <template <typename> class VecCls1, typename VecType1, template <typename> class VecCls2, typename VecType2> static VecCls1<VecType1> Add(const VecCls1<VecType1> &lhs, const VecCls2<VecType2> &rhs)
  {
    const int64_t maxElem = atMin(lhs.element_count, rhs.element_count);
    VecCls1<VecType1> ret = lhs;
    for (int64_t i = 0; i < maxElem; ++i)
      ret[i] = (VecType1)lhs[i] + (VecType1)rhs[i];
    return ret;
  }
  
  template <template <typename> class VecCls1, typename VecType1, template <typename> class VecCls2, typename VecType2> static VecCls1<VecType1> Subtract(const VecCls1<VecType1> &lhs, const VecCls2<VecType2> &rhs)
  {
    const int64_t maxElem = atMin(lhs.element_count, rhs.element_count);
    VecCls1<VecType1> ret = lhs;
    for (int64_t i = 0; i < maxElem; ++i)
      ret[i] = (VecType1)lhs[i] - (VecType1)rhs[i];
    return ret;
  }
  
  template <template <typename> class VecCls1, typename VecType1, template <typename> class VecCls2, typename VecType2> static VecCls1<VecType1> Multiply(const VecCls1<VecType1> &lhs, const VecCls2<VecType2> &rhs)
  {
    const int64_t maxElem = atMin(lhs.element_count, rhs.element_count);
    VecCls1<VecType1> ret = lhs;
    for (int64_t i = 0; i < maxElem; ++i)
      ret[i] = (VecType1)lhs[i] * (VecType1)rhs[i];
    return ret;
  }
  
  template <template <typename> class VecCls1, typename VecType1, template <typename> class VecCls2, typename VecType2> static VecCls1<VecType1> Divide(const VecCls1<VecType1> &lhs, const VecCls2<VecType2> &rhs)
  {
    const int64_t maxElem = atMin(lhs.element_count, rhs.element_count);
    VecCls1<VecType1> ret = lhs;
    for (int64_t i = 0; i < maxElem; ++i)
      ret[i] = (VecType1)lhs[i] / (VecType1)rhs[i];
    return ret;
  }

  template <template <typename> class VecCls1, typename VecType1, template <typename> class VecCls2, typename VecType2> static VecCls1<VecType1> Assign(VecCls1<VecType1> *pLhs, const VecCls2<VecType2> &rhs)
  {
    const int64_t maxElem = atMin(pLhs->element_count, rhs.element_count);
    for (int64_t i = 0; i < maxElem; ++i)
      (*pLhs)[i] = (VecType1)rhs[i];
    return *pLhs;
  }

  template <template <typename> class VecCls1, typename VecType1, template <typename> class VecCls2, typename VecType2> static bool Equals(const VecCls1<VecType1> &lhs, const VecCls2<VecType2> &rhs)
  {
    if (lhs.element_count != rhs.element_count)
      return false;
    for (int64_t i = 0; i < lhs.element_count; ++i)
      if (lhs[i] != (VecType1)rhs[i])
        return false;
    return true;
  }

  template <template <typename> class VecCls1, typename VecType1, template <typename> class VecCls2, typename VecType2> static bool NotEqual(const VecCls1<VecType1> &lhs, const VecCls2<VecType2> &rhs) { return !Equals(lhs, rhs); }

  //----------------------
  // Vector Math Functions

  template <template <typename> class VecCls1, typename VecType1, template <typename> class VecCls2, typename VecType2> static VecType1 Angle(const VecCls1<VecType1> &lhs, const VecCls2<VecType2> &rhs) { return atCos(Dot(lhs, rhs) / (Mag(lhs) * (VecType1)Mag(rhs))); }

  template <template <typename> class VecCls1, typename VecType1, template <typename> class VecCls2, typename VecType2> static VecType1 Dot(const VecCls1<VecType1> &lhs, const VecCls2<VecType2> &rhs)
  {
    atAssert(lhs.element_count == rhs.element_count, "atVectorMath::Dot() - Vectors must have the same element count.");
    VecType1 ret = { 0 };
    for (int64_t i = 0; i < lhs.element_count; ++i)
      ret += lhs[i] * (VecType1)rhs[i];
    return ret;
  }

  template <template <typename> class VecCls1, typename VecType1, template <typename> class VecCls2, typename VecType2> static VecCls1<VecType1> Cross(const VecCls1<VecType1> &lhs, const VecCls2<VecType2> &rhs)
  {
    atAssert(lhs.element_count == 3 && rhs.element_count == 3,"atVectorMath::Cross() - Vectors must have an element count of 3.");
    return VecCls1<VecType1>(lhs[1] * rhs[2] - lhs[2] * rhs[1], lhs[2] * rhs[0] - lhs[0] * rhs[2], lhs[0] * rhs[1] - lhs[1] * rhs[0]);
  }


  template <template <typename> class VecCls, typename VecType> static VecType Length(const VecCls<VecType> &vec)
  {
    VecType ret = { 0 };
    for (int64_t i = 0; i < vec.element_count; ++i)
      ret += vec[i] * vec[i];
    return ret;
  }

  template <template <typename> class VecCls, typename VecType> static VecType Mag(const VecCls<VecType> &vec) { return sqrt(Length(vec)); }
  template <template <typename> class VecCls, typename VecType> static VecCls<VecType> Normalize(const VecCls<VecType> &vec) { VecType mag = Mag(vec); return Divide(vec, mag == 0 ? 1 : mag); }
};

#endif // atVectorMath_h__
