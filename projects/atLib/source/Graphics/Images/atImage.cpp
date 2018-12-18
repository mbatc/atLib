
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

#include "atImage.h"

static atCol _SampleNearest(const atVec2F &uv, const atVector<atCol> &pixels, const int64_t w, const int64_t h) { return pixels[(int64_t)(uv.x * w + 0.5f) + (int64_t)(uv.y * h + 0.5f) * w]; }

static atCol _SampleBilinear(const atVec2F &uv, const atVector<atCol> &pixels, const int64_t w, const int64_t h)
{
  int64_t x = (int64_t)(uv.x * w);
  int64_t y = (int64_t)(uv.y * h);
  atVec2F _min = atVec2F{ (float)x / (float)w, (float)y / (float)h };
  atVec2F _max = atVec2F{ (float)(x + 1) / (float)w, (float)(y + 1) / (float)h };
  atVec2F _lower = (_max - uv) / (_max - _min);
  atVec2F _upper = (uv - _min) / (_max - _min);

  atVec4F a = atColor::UnPack<float>(pixels[x + y * w]) * (1.f - _lower.x) * (1.f - _lower.y);
  atVec4F b = atColor::UnPack<float>(pixels[x + (y + 1) * w]) * (1.f - _lower.x) * (1.f - _upper.y);
  atVec4F c = atColor::UnPack<float>(pixels[x + 1 + y * w]) * (1.f - _upper.x) * (1.f - _lower.y);
  atVec4F d = atColor::UnPack<float>(pixels[x + 1 + (y + 1) * w]) * (1.f - _upper.x) * (1.f - _upper.y);

  return atColor::Pack(a + b + c + d);
}

atImage::atImage(const atFilename &file) { m_pixels = std::move(atImageHelper::LoadImage(file, &m_size)); }
const atCol & atImage::Get(const int64_t x, const int64_t y) const { return m_pixels[x + y * Width()]; }
atImage::atImage(const atVector<atCol> &data, const atVec2I &size) : m_pixels(data), m_size(size) {}
atCol& atImage::Get(const int64_t x, const int64_t y) { return m_pixels[x + y * Width()]; }
atImage::atImage(const atVec2I &size) : m_pixels(size.x * size.y, 0), m_size(size) {}
const atCol & atImage::operator[](const int64_t i) const { return m_pixels[i]; }
atCol & atImage::operator[](const int64_t i) { return m_pixels[i]; }
const atVector<atCol>& atImage::Pixels() const { return m_pixels; }
const int64_t atImage::Height() const { return m_size.y; }
const int64_t atImage::Width() const { return m_size.x; }
const atVec2I& atImage::Size() const { return m_size; }
atVector<atCol>& atImage::Pixels() { return m_pixels; }
atImage::atImage() {}

atImage::atImage(atImage &&move)
  : m_pixels(std::move(move.m_pixels))
  , m_size(move.m_size)
{
  move.m_size = atVec2I::zero();
}

atImage::atImage(const atImage &copy)
  : m_pixels(copy.m_pixels)
  , m_size(copy.m_size)
{}

atImage atImage::Resize(const atVec2I &size, const atSampleType sampler) const 
{
  atImage ret(size);
  for (int64_t y = 0; y < ret.Height(); ++y)
    for (int64_t x = 0; x < ret.Width(); ++x)
      ret.Get(x, y) = Sample(atVec2F((float)x / (float)ret.Width(), (float)y / (float)ret.Height()), sampler);
  return ret;
}

atImage atImage::Pad(const int64_t top, const int64_t left, const int64_t bottom, const int64_t right) const
{
  atImage ret(atVec2I(Width() + left + right, Height() + top + bottom));
  for (int64_t y = 0; y < ret.Height(); ++y)
    for (int64_t x = 0; x < ret.Width(); ++x)
      ret.Get(x, y) = Get(atMin(atMax(-left + x, 0), Width() - 1), atMin(atMax(-top + y, 0), Height() - 1));
  return ret;
}

atCol atImage::Sample(const atVec2F &uv, const atSampleType type) const
{
  switch (type)
  {
  case atST_Nearest: return _SampleNearest(uv, m_pixels, Width(), Height());
  case atST_Bilinear: return _SampleBilinear(uv, m_pixels, Width(), Height());
  }
  return 0;
}

const atImage &atImage::operator=(const atImage &copy)
{
  m_pixels = copy.m_pixels;
  m_size = copy.m_size;
  return *this;
}

const atImage & atImage::operator=(atImage &&move)
{
  m_pixels = std::move(move.m_pixels);
  m_size = move.m_size;
  move.m_size = atVec2I::zero();
  return *this;
}
