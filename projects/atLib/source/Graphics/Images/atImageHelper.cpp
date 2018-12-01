
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

#include "atImageHelper.h"

//---------------------
// THIRD PARTY INCLUDES

#include "stb_image_write.h" // https://github.com/nothings/stb | stb single-file public domain libraries for C/C++ https://twitter.com/nothings
#include "stb_image.h"       // https://github.com/nothings/stb | stb single-file public domain libraries for C/C++ https://twitter.com/nothings

//---------------------

static atCol _LoadChannel1(const uint8_t *pData) { return atColor::Pack(pData[0], pData[0], pData[0]); }
static atCol _LoadChannel3(const uint8_t *pData) { return atColor::Pack(pData[0], pData[1], pData[2]); }
static atCol _LoadChannel2(const uint8_t *pData) { return atColor::Pack(pData[0], pData[0], pData[0], pData[1]); }
static atCol _LoadChannel4(const uint8_t *pData) { return *(atCol*)pData; }

static atVector<atCol> _LoadFromMemory(const uint8_t *pData, const atVec2I &size, const int nChannels)
{
  atVector<atCol> data;
  atCol (*load_func)(const uint8_t*) = 0;
  if (nChannels == 1) load_func = _LoadChannel1;
  if (nChannels == 2) load_func = _LoadChannel2;
  if (nChannels == 3) load_func = _LoadChannel3;
  if (nChannels == 4) load_func = _LoadChannel4;
  if (!load_func) return data;
  data.reserve(size.x * size.y);
  for (int64_t y = 0; y < size.y; ++y)
    for (int64_t x = 0; x < size.y; ++x)
      data.push_back(load_func(pData + (x * nChannels + y * size.x * nChannels)));
  return data;
}

atVector<atCol> atImageHelper::LoadImage(const atFilename &file, atVec2I *pSize)
{
  atVec2I size;
  int nChannels = 0;
  uint8_t *pData = stbi_load(file.c_str(), &size.x, &size.y, &nChannels, 0);
  atVector<atCol> rawImage = _LoadFromMemory(pData, size, nChannels);
  stbi_image_free(pData);
  *pSize = size;
  return rawImage;
}

bool atImageHelper::SaveImage(const atVector<atCol> &data, const atVec2I &size, const atImageFormat format, const atFilename &file)
{
  switch (format)
  {
  case atIF_JPEG: return stbi_write_jpg(file.c_str(), size.x, size.y, 4, data.data(), 100) > 0;
  case atIF_PNG: return stbi_write_png(file.c_str(), size.x, size.y, 4, data.data(), size.x * 4) > 0;
  case atIF_BMP: return stbi_write_bmp(file.c_str(), size.x, size.y, 4, data.data()) > 0;
  case atIF_TGA: return stbi_write_tga(file.c_str(), size.x, size.y, 4, data.data()) > 0;
  }
  return false;
}
