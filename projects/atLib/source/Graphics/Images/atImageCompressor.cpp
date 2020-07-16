#include "atImageCompressor.h"

#define PIXEL_MASK uint32_t(~(1 | (1 << 8) | (1 << 16) | (1 << 24) | (0xFF << 24)))
#define IS_PIXEL(byte) ((byte & 1) == 0)

static void _PackColour(uint8_t **ppBuffer, atCol colour)
{
  *((*ppBuffer)++) = atCOL_GETR(colour) & ~(1);
  *((*ppBuffer)++) = atCOL_GETG(colour) & ~(1);
  *((*ppBuffer)++) = atCOL_GETB(colour) & ~(1);
}

static atCol _UnpackColour(const uint8_t **ppBuffer)
{
  atCol colour = atCOL_PACKA(0xFF);
  colour |= atCOL_PACKR(*((*ppBuffer)++));
  colour |= atCOL_PACKG(*((*ppBuffer)++));
  colour |= atCOL_PACKB(*((*ppBuffer)++));
  return colour;
}

static void _PackCount(uint8_t **ppBuffer, uint64_t value)
{
  while (value > 0)
  {
    uint8_t byte = ((value << 1) & 0xFF) | 1;
    value = value >> 7;
    *((*ppBuffer)++) = byte;
  }
}

static uint64_t _UnpackCount(const uint8_t **ppBuffer, const uint8_t *pBufferEnd)
{
  int64_t bitOffset = 0;
  int64_t value = 0;
  while (*ppBuffer < pBufferEnd)
  {
    uint8_t byte = **ppBuffer;
    if (IS_PIXEL(byte))
      break;
    value += ((byte & ~(1)) >> 1) << bitOffset;
    bitOffset += 7;
    (*ppBuffer)++;
  }

  return value;
}

static void _PackInt(uint8_t **ppBuffer, uint32_t val)
{
  memcpy(*ppBuffer, &val, sizeof(uint32_t));
  *ppBuffer += sizeof(uint32_t);
}

static uint32_t _UnpackInt(const uint8_t **ppBuffer)
{
  uint32_t val;
  memcpy(&val, *ppBuffer, sizeof(uint32_t));
  *ppBuffer += sizeof(uint32_t);
  return val;
}

atVector<uint8_t> atImageCompressor::Compress(const atImage &image)
{
  // Calculate a buffer large enough
  int64_t headerSize = sizeof(uint32_t) * 2;
  int64_t maxCompressedSize = image.Size().x * image.Size().y * 3;
  int64_t capacity = headerSize + maxCompressedSize;

  // Allocate the buffer
  uint8_t *pBuffer = (uint8_t*)atAlloc(capacity);
  uint8_t *pCursor = pBuffer;
  
  // Write the header
  _PackInt(&pCursor, (uint32_t)image.Size().x);
  _PackInt(&pCursor, (uint32_t)image.Size().y);

  // Compress and write the image
  atCol lastCol = 0;
  int64_t count = -1;
  atCol pixelMask = PIXEL_MASK;
  for (const atCol &col : image.Pixels())
  {
    if (count < 0 || (col & pixelMask) != lastCol)
    {
      // If there is more than 1 pixel, pack the count
      if (count > 1)
        _PackCount(&pCursor, count);

      // Get the colour and add it to the compressed image stream
      lastCol = col & pixelMask;
      _PackColour(&pCursor, lastCol);
      count = 0;
    }

    ++count;
  }

  if (count > 1)
    _PackCount(&pCursor, count);

  atVector<uint8_t> ret;
  ret.set_data(pBuffer, pCursor - pBuffer, capacity);
  return ret;
}

atImage atImageCompressor::Decompress(const atVector<uint8_t> &data)
{
  int64_t pixelIndex = 0;

  const uint8_t *pBuffer = data.begin();
  const uint8_t *pEnd = data.end();

  uint32_t width = _UnpackInt(&pBuffer);
  uint32_t height = _UnpackInt(&pBuffer);

  atImage image(atVec2I(width, height));
  atCol lastPixel = 0;
  while (pBuffer < pEnd)
  {
    if (IS_PIXEL(*pBuffer))
      lastPixel = _UnpackColour(&pBuffer);
    int64_t count = 1;
    if (pBuffer < pEnd && !IS_PIXEL(*pBuffer))
      count = _UnpackCount(&pBuffer, pEnd);
    for (int64_t p = 0; p < count; ++p)
      image[pixelIndex++] = lastPixel;
  }

  return image;
}
