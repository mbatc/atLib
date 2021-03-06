
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
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

#ifndef atColor_h__
#define atColor_h__

#include "atMath.h"

#define atCOL_MASKR(c) (0x000000FF & (c))
#define atCOL_MASKG(c) (0x0000FF00 & (c))
#define atCOL_MASKB(c) (0x00FF0000 & (c))
#define atCOL_MASKA(c) (0xFF000000 & (c))

#define atCOL_GETR(c) (atCOL_MASKR(c))
#define atCOL_GETG(c) (atCOL_MASKR(c >> 8))
#define atCOL_GETB(c) (atCOL_MASKR(c >> 16))
#define atCOL_GETA(c) (atCOL_MASKR(c >> 24))

#define atCOL_PACKR(c) (atCOL_MASKR(c))
#define atCOL_PACKG(c) (atCOL_MASKR(c) << 8)
#define atCOL_PACKB(c) (atCOL_MASKR(c) << 16)
#define atCOL_PACKA(c) (atCOL_MASKR(c) << 24)

#define atCOL_PACK_RGBA(r, g, b, a) (atCOL_PACKR(r) | atCOL_PACKG(g) | atCOL_PACKB(b) | atCOL_PACKA(a))

typedef uint32_t atCol;

class atColor
{
public:
  atColor() = delete;

  // Unpacks an ARGB atCol to a RGBA atVector4
  template<typename T> static atVector4<T> UnPackARGB(atCol col);

  // Unpacks an RGBA atCol to a RGBA atVector4
  template<typename T> static atVector4<T> UnPack(atCol col);


  // Packs an RGBA atVector4 to a RGBA atCol
  template<typename T> static atCol Pack(const atVector4<T> &col);

  // Packs an RGB atVector3 to a RGBA atCol with A = 255
  template<typename T> static atCol Pack(const atVector3<T> &col);
  template<typename T> static atCol Pack(T r, T g, T b, T a = (T)255);


  // Packs an RGB atVector4 to a ARGB atCol with A
  template<typename T> static atCol PackARGB(const atVector4<T> &col);

  // Packs an RGB atVector3 to a ARGB atCol with A = 255
  template<typename T> static atCol PackARGB(const atVector3<T> &col);
  template<typename T> static atCol PackARGB(T r, T g, T b, T a = (T)255);

  // Non-Reversible
  static atCol RGBAtoARGB(atCol col);
  static atCol ARGBtoRGBA(atCol col);

  // Reversible
  static atCol RGBAtoBGRA(atCol col);

  static atCol black;
  static atCol white;
  static atCol red;
  static atCol green;
  static atCol blue;
};

// Specialization for floating point values (Color values range from 0-1 instead of 0-255)

template<> atCol atColor::Pack<float>(float r, float g, float b, float a);
template<> atCol atColor::Pack<double>(double r, double g, double b, double a);
template<> atCol atColor::PackARGB<float>(float r, float g, float b, float a);
template<> atCol atColor::PackARGB<double>(double r, double g, double b, double a);

template<> atVector4<double> atColor::UnPackARGB(atCol col);
template<> atVector4<double> atColor::UnPack(atCol col);

template<> atVector4<float> atColor::UnPackARGB(atCol col);
template<> atVector4<float> atColor::UnPack(atCol col);

#include "atColor.inl"
#endif // atColor_h__
