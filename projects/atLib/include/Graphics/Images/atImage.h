
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

#ifndef atImage_h__
#define atImage_h__

#include "atImageHelper.h"

enum atSampleType
{
  atST_Nearest,
  atST_Bilinear
};

class atImage
{
public:
  atImage();
  atImage(const atVec2I &size, const atCol &colour = atColor::black);
  atImage(const atFilename &file);
  atImage(const atCol *pPixels, const atVec2I &size);
  atImage(const atVector<atCol> &data, const atVec2I &size);
  atImage(const uint8_t *pPixels, const atVec2I &size, const int64_t nChannels = 4);

  atImage(atImage &&move);
  atImage(const atImage &copy);

  atVector<atCol>& Pixels();
  const atVector<atCol>& Pixels() const;

  const atCol& Get(const int64_t x, const int64_t y) const;
  atCol& Get(const int64_t x, const int64_t y);
  const atCol& operator[](const int64_t i) const;
  atCol& operator[](const int64_t i);

  const atVec2I& Size() const;
  const int64_t Width() const;
  const int64_t Height() const;

  atImage Resize(const atVec2I &size, const atSampleType sampler) const;
  atImage Pad(const int64_t top, const int64_t left, const int64_t bottom, const int64_t right) const;

  atCol Sample(const atVec2F &uv, const atSampleType type = atST_Bilinear) const;

  const atImage& operator=(const atImage &copy);
  const atImage& operator=(atImage &&move);

protected:
  atVec2I m_size;
  atVector<atCol> m_pixels;
};

#endif // atImage_h__
