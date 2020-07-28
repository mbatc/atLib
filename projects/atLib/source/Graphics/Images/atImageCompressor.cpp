#include "atImageCompressor.h"

#define PIXEL_MASK(quality) uint32_t(~atCOL_PACK_RGBA(quality, quality, quality,  255))
#define IS_PIXEL(byte) ((byte & 1) == 0)

// Compression instructions are encoded into the the 2nd least significant bit
// of the R channel and the 2 least significant bits of the G and B channels.
//
// The encoding can support a maximum of 32 different instructions directly encoded
// into pixel data.
//
// There is potential to extent this limit if an instruction is reserved for appending
// and extra byte to encode the pixel into.

enum CompressionInstruction : uint8_t
{
  CI_Horizontal, // Compress similar pixels iterating along the x-axis
  CI_Vertical,   // Compress similar pixels iterating along the y-axis
  CI_Square,     // Compress similar pixels iterating along both the x and y axis
  CI_Count,
};

struct Block
{
  atVec4D colour           = 0;       // Average colour of the block
  int64_t count            = 0;       // Decompression argument
  int64_t pixelsCompressed = 0;       // Total pixels in the block
  CompressionInstruction instruction; // Compressed block type

  atVec2I coord = 0;           // Start coordinate of the block
  const atCol *pNextPixel = 0; // No 
};

struct CompressorContext
{
  const atCol *pPixels = 0;
  const atCol *pCurrentPixel = 0;
  const atCol *pLastPixel = 0;
  atCol pixelMask = PIXEL_MASK(3);

  int64_t stride   = 0;
  atVec2I size     = 0;

  uint8_t *pStart  = 0;
  uint8_t *pCursor = 0;

  // Lookup of compressed pixels
  bool *pCompressed = 0;

  // Blocks to write out
  // atVector<Block> blocks;

  // TODO: Lookup for chaining colour blocks together.
  // atHashMap<atCol, int64_t> lastBlock;
};

// Encode and instruction into a pixel
static atCol _EncodeInstruction(atCol colour, uint8_t instruction)
{
  static const uint32_t pixelMask = PIXEL_MASK(3);

  // Clear the 2 least significant bits of each byte
  colour = colour & pixelMask;

  if (instruction > 0)
  {
    colour |= (instruction & 1) << 1;
    instruction = instruction >> 1;
  }
  else
  {
    return colour;
  }

  int64_t bitOffset = 8;
  while (instruction > 0)
  {
    colour |= (instruction & 3) << bitOffset;
    instruction = instruction >> 2;
    bitOffset += 8;
  }

  return colour;
}

// Writing/Reading data
static void _PackColour(uint8_t **ppBuffer, atCol colour)
{
  *((*ppBuffer)++) = atCOL_GETR(colour);
  *((*ppBuffer)++) = atCOL_GETG(colour);
  *((*ppBuffer)++) = atCOL_GETB(colour);
}

static void _PackArg(uint8_t **ppBuffer, uint64_t value)
{
  while (value > 0)
  {
    uint8_t byte = ((value << 1) & 0xFF) | 1;
    value = value >> 7;
    *((*ppBuffer)++) = byte;
  }
}

static void _PackInt(uint8_t **ppBuffer, uint32_t val)
{
  memcpy(*ppBuffer, &val, sizeof(uint32_t));
  *ppBuffer += sizeof(uint32_t);
}

static void _FlagPixels(bool *pFlags, const atVec2I &size, const Block &block)
{
  switch (block.instruction)
  {
  case CI_Horizontal:
  {
    bool *pFlag = pFlags + size.x * block.coord.y + block.coord.x;
    for (int64_t i = 0; i < block.count; ++i)
    {
      atRelAssert(pFlag - pFlags < size.x * size.y, "OOPS");
      *(pFlag++) = true;
    }
  } break;
  case CI_Vertical:
  {
    atVec2I endCoord = atVec2I(block.coord.x + (block.coord.y + block.count) / size.y, (block.coord.y + block.count) % size.y);

    // Write 'count' pixels iterating vertically
    for (int64_t x = block.coord.x; x <= endCoord.x; ++x)
    {
      int64_t maxY = x + 1 >= endCoord.x ? endCoord.y : size.y;
      for (int64_t y = block.coord.y; y < maxY; ++y)
        pFlags[x + y * size.x] = true;
    }
  } break;
  case CI_Square:
  {
    atVec2I endCoord = block.coord + block.count;
    for (int64_t y = block.coord.y; y < endCoord.y; ++y)
      for (int64_t x = block.coord.x; x < endCoord.x; ++x)
        pFlags[x + y * size.x] = true;
  } break;
  }
}

static void _WriteBlock(CompressorContext *pCtx, const Block &block)
{
  _PackColour(&pCtx->pCursor, _EncodeInstruction(atColor::Pack(block.colour), block.instruction));
  if (block.count> 1)
  {
    _PackArg(&pCtx->pCursor, block.count);
    _FlagPixels(pCtx->pCompressed, pCtx->size, block);
  }
  else
  {
    pCtx->pCompressed[pCtx->pCurrentPixel - pCtx->pPixels] = true;
  }
}

// Compression functions

static Block _TryCompressHorizontal(CompressorContext *pContext)
{
  Block block;
  block.count = 0;

  const atCol *pPixel = pContext->pCurrentPixel;
  atCol lastCol = *pPixel & pContext->pixelMask; 
  while (pPixel < pContext->pLastPixel)
  {
    int64_t index = pPixel - pContext->pPixels;
    if (pContext->pCompressed[index])
    { // Skip this pixel because its already been compressed
      ++pPixel;
      continue;
    }

    if ((*pPixel & pContext->pixelMask) != lastCol)
      break;

    block.colour += atColor::UnPack<double>(*(pPixel++));
    ++block.count;
  }

  block.pixelsCompressed = block.count;
  block.pNextPixel = pPixel;
  block.instruction = CI_Horizontal;
  block.colour /= block.pixelsCompressed;
  return block;
}

static Block _TryCompressVertical(CompressorContext *pContext)
{
  Block block;
  block.count = 0;

  // Get the comparison colour from the current pixel
  const atCol *pPixel = pContext->pCurrentPixel;
  atCol lastCol = *pPixel & pContext->pixelMask;

  // Get the column of the current pixel
  int64_t pixelIdx = int64_t(pPixel - pContext->pPixels);
  int64_t column = pixelIdx % pContext->size.x;
  int64_t startCol = column;
  
  // Move to the next pixel
  pPixel += pContext->stride;

  while (pPixel < pContext->pLastPixel && column < pContext->size.x)
  {
    int64_t index = pPixel - pContext->pPixels;
    if (!pContext->pCompressed[index])
    {
      if ((*pPixel & pContext->pixelMask) != lastCol)
        break;

      // Count this pixel
      ++block.count;

      // Add the colours as floating-point values so we can store the average of the block.
      // This is done in case a lossy compression method is used.
      block.colour += atColor::UnPack<double>(*pPixel);
    }

    // Move to the next row
    pPixel += pContext->stride;

    // Check if the end of the column has been reached - move to the next column
    if (pPixel > pContext->pLastPixel)
      pPixel = pContext->pCurrentPixel + (++column);
  }

  // Fill out the rest of the blocks details
  block.pixelsCompressed = block.count;

  // The next byte is 1 pixel horizontally from end of this block on the starting row.
  // If we have gone past the right bound of the image, set the next pixel to the pixel below the last
  // pixel in this block
  block.pNextPixel = pContext->pCurrentPixel + column - startCol + 1;

  block.instruction = CI_Vertical;
  block.colour /= block.pixelsCompressed;
  return block;
}

static Block _TryCompressSquare(CompressorContext *pContext)
{
  Block block;
  block.count = 0;

  // Get the colour to compare to
  atCol lastCol = *pContext->pCurrentPixel & pContext->pixelMask;

  // Get the coordinate of the current pixel
  int64_t firstIdx = int64_t(pContext->pCurrentPixel - pContext->pPixels);
  atVec2I coord = atFromIndex<int64_t>(firstIdx, pContext->size);

  // Get the blocks start/end coordinate
  atVec2I startCoord = coord;
  int64_t count = 1;
  while (true)
  {
    atVec4D avgColour = 0;
    atVec2I endCoord = startCoord + count + 1;
    if (endCoord.x >= pContext->size.x || endCoord.y >= pContext->size.y)
      break;

    // Iterate horizontally
    bool differenceFound = false;
    for (int64_t x = startCoord.x; x < endCoord.x; ++x)
    {
      int64_t pixelIndex = atToIndex<int64_t>({ x, endCoord.y - 1 }, pContext->size);
      if (pContext->pCompressed[pixelIndex])
        continue;

      atCol colour = pContext->pPixels[pixelIndex];
      if (lastCol != (colour & pContext->pixelMask))
      {
        differenceFound = true;
        break;
      }

      avgColour += atColor::UnPack<double>(colour);
    }

    if (differenceFound)
      break;

    // Iterate vertically
    for (int64_t y = startCoord.y; y < endCoord.y; ++y)
    {
      int64_t pixelIndex = atToIndex<int64_t>({ endCoord.x - 1, y }, pContext->size);
      if (pContext->pCompressed[pixelIndex])
        continue;

      atCol colour = pContext->pPixels[pixelIndex];
      if (lastCol != (colour & pContext->pixelMask))
      {
        differenceFound = true;
        break;
      }
      avgColour += atColor::UnPack<double>(colour);
    }

    if (differenceFound)
      break;

    block.colour += avgColour;
    ++count;
  }

  // Block pixel count
  block.count = count;
  block.pixelsCompressed = block.count * block.count;

  // Find the next pixel to start compressing from
  atVec2I nextPixelCoord;
  atVec2I endCoord = startCoord + count;
  if (endCoord.x < pContext->size.x)
    nextPixelCoord = atVec2I(endCoord.x, startCoord.y);
  else
    nextPixelCoord = atVec2I(startCoord.x, endCoord.y);

  block.pNextPixel = atMin(pContext->pLastPixel, pContext->pPixels + atToIndex(nextPixelCoord, pContext->size));
  block.instruction = CI_Square;
  block.colour /= block.pixelsCompressed;
  return block;
}

static Block _TryCompress(CompressorContext *pContext, const CompressionInstruction &instr)
{
  switch (instr)
  {
  case CI_Horizontal: return _TryCompressHorizontal(pContext);
  case CI_Vertical: return _TryCompressVertical(pContext); 
  case CI_Square: return _TryCompressSquare(pContext);
  }

  return Block();
}

atVector<uint8_t> atImageCompressor::Compress(const atImage &image, const int64_t &quality)
{
  // Calculate a buffer large enough
  int64_t headerSize = sizeof(uint32_t) * 2;
  int64_t maxCompressedSize = image.Size().x * image.Size().y * 3;
  int64_t capacity = headerSize + maxCompressedSize;

  // Setup the compression context
  CompressorContext ctx;
  ctx.stride = image.Size().x * sizeof(atCol);
  ctx.pPixels = image.Pixels().data();
  ctx.pCurrentPixel = ctx.pPixels;
  ctx.pLastPixel = &image.Pixels().back();
  ctx.size = image.Size();
  ctx.pCompressed = (bool*)atAlloc(image.Size().x * image.Size().y);
  memset(ctx.pCompressed, 0, image.Size().x * image.Size().y);

  uint8_t byteMask = 0;
  for (int64_t b = 0; b < quality; ++b)
    byteMask |= 1 << b;
  ctx.pixelMask = PIXEL_MASK(byteMask);

  // Allocate the compressed buffer
  ctx.pStart = (uint8_t*)atAlloc(capacity);
  ctx.pCursor = ctx.pStart;
  
  // Write the header
  _PackInt(&ctx.pCursor, (uint32_t)image.Size().x);
  _PackInt(&ctx.pCursor, (uint32_t)image.Size().y);

  while (ctx.pCurrentPixel < ctx.pLastPixel)
  {
    // Skip pixels that have already been compressed
    if (ctx.pCompressed[ctx.pCurrentPixel - ctx.pPixels])
    {
      ++ctx.pCurrentPixel;
      continue;
    }

    // Block bestBlock = _TryCompress(&ctx, CI_Horizontal);
    Block bestBlock = _TryCompress(&ctx, CI_Vertical);
    // Block bestBlock = _TryCompress(&ctx, CI_Square);

    // Test each of the supported instructions and use which ever compresses
    // the most pixels
    // for (int64_t instr = 0; instr < CI_Count; ++instr)
    // {
    //   Block testBlock = _TryCompress(&ctx, (CompressionInstruction)instr);
    //   if (testBlock.pixelsCompressed > bestBlock.pixelsCompressed)
    //     bestBlock = testBlock;
    // }


    // Write the block to the compressed buffer
    if (bestBlock.count > 0)
    {
      bestBlock.coord = atFromIndex<int64_t>(int64_t(ctx.pCurrentPixel - ctx.pPixels), ctx.size);
      _WriteBlock(&ctx, bestBlock);
    }

    // Set the current pixel to the pixel following the best block
    ctx.pCurrentPixel = bestBlock.pNextPixel;
  }

  atFree(ctx.pCompressed);

  // Simple compression implementation
  // atCol lastCol = 0;
  // int64_t count = -1;
  // atVec3D channels(0);
  //
  // for (const atCol &col : image.Pixels())
  // {
  //   atCol testCol = col & ctx.pixelMask;
  //   if (count < 0 || testCol != lastCol)
  //   {
  //     if (count > 0)
  //       _WriteBlock(&ctx.pCursor, atColor::Pack(channels / count), CI_Horizontal, count);
  // 
  //     channels = atVec3D(0);
  //     count = 0;
  //     lastCol = testCol;
  //   }
  // 
  //   channels += atColor::UnPack<double>(col).xyz();
  //   ++count;
  // }

  // if (count > 0)
  //   _WriteBlock(&ctx.pCursor, atColor::Pack(channels / count), instruction, count);

  atVector<uint8_t> ret;
  ret.set_data(ctx.pStart, ctx.pCursor - ctx.pStart, capacity);
  return ret;
}

struct DecompressorContext
{
  const uint8_t *pBuffer      = 0;
  const uint8_t *pBufferStart = 0;
  const uint8_t *pBufferEnd   = 0;
  bool *pDecompressed         = 0;
  int64_t cursor              = 0;

  atCol lastPixel = 0;

  atImage image;
};

static uint8_t _DecodeInstruction(atCol colour)
{
  uint8_t instruction = 0;
  instruction |= uint8_t((colour & 2) >> 1);
  instruction |= uint8_t((colour & (3 << 8)) >> 7);
  instruction |= uint8_t((colour & (3 << 16)) >> 13);
  return instruction;
}

static atCol _UnpackColour(const uint8_t **ppBuffer, CompressionInstruction *pInstruction)
{
  static atCol pixelMask = PIXEL_MASK(3);

  // Read the pixel data
  atCol colour = atCOL_PACKA(0xFF);
  colour |= atCOL_PACKR(*((*ppBuffer)++));
  colour |= atCOL_PACKG(*((*ppBuffer)++));
  colour |= atCOL_PACKB(*((*ppBuffer)++));

  // Decode the instruction
  *pInstruction = (CompressionInstruction)_DecodeInstruction(colour);

  // Return the colour without the instruction bits
  return colour & pixelMask | atCOL_PACKA(0xFF);
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

static uint32_t _UnpackInt(const uint8_t **ppBuffer)
{
  uint32_t val;
  memcpy(&val, *ppBuffer, sizeof(uint32_t));
  *ppBuffer += sizeof(uint32_t);
  return val;
}

static void _DecompressHorizontal(DecompressorContext *pCtx, const int64_t &count)
{
  // Write 'count' pixels iterating horizontally
  int64_t end = pCtx->cursor + count;
  while (pCtx->cursor < end)
    if (!pCtx->pDecompressed[pCtx->cursor])
    {
      pCtx->pDecompressed[pCtx->cursor] = true;
      pCtx->image[pCtx->cursor++] = pCtx->lastPixel;
    }
}

static void _DecompressVertical(DecompressorContext *pCtx, const int64_t &count)
{
  atVec2I startCoord = atFromIndex<int64_t>(pCtx->cursor, pCtx->image.Size());
  atVec2I endCoord = atVec2I(startCoord.x + (startCoord.y + count) / pCtx->image.Size().y, (startCoord.y + count) % pCtx->image.Size().y);

  // Write 'count' pixels iterating vertically
  for (int64_t x = startCoord.x; x <= endCoord.x; ++x)
  {
    int64_t maxY = x + 1 >= endCoord.x ? endCoord.y : pCtx->image.Size().y;
    for (int64_t y = startCoord.y; y < maxY; ++y)
    {
      int64_t index = atToIndex({ x, y }, pCtx->image.Size());
      if (pCtx->pDecompressed[index])
        continue;
      pCtx->pDecompressed[index] = true;
      pCtx->image[index] = pCtx->lastPixel;
    }
  }

  // Place the cursor 
  pCtx->cursor = atToIndex({ endCoord.x + 1, startCoord.y }, pCtx->image.Size());
}

static void _DecompressSquare(DecompressorContext *pCtx, const int64_t &count)
{
  atVec2I startCoord = atFromIndex<int64_t>(pCtx->cursor, pCtx->image.Size());
  atVec2I endCoord = startCoord + count;

  for (int64_t y = startCoord.y; y < endCoord.y; ++y)
    for (int64_t x = startCoord.x; x < endCoord.x; ++x)
    {
      int64_t index = atToIndex({ x, y }, pCtx->image.Size());
      if (pCtx->pDecompressed[index])
        continue;
      pCtx->image[index] = pCtx->lastPixel;
      pCtx->pDecompressed[index] = true;
    }

  if (endCoord.x < pCtx->image.Size().x)
    pCtx->cursor = atToIndex({ endCoord.x, startCoord.y }, pCtx->image.Size());
  else
    pCtx->cursor = atToIndex({ startCoord.x, endCoord.y }, pCtx->image.Size());
}

static void _DecompressBlock(DecompressorContext *pCtx, const CompressionInstruction &instruction, const int64_t &count)
{
  switch (instruction)
  {
  case CI_Horizontal: _DecompressHorizontal(pCtx, count); break;
  case CI_Vertical: _DecompressVertical(pCtx, count); break;
  case CI_Square: _DecompressSquare(pCtx, count); break;
  }
}

atImage atImageCompressor::Decompress(const atVector<uint8_t> &data)
{
  int64_t pixelIndex = 0;

  DecompressorContext ctx;
  ctx.pBuffer = data.begin();
  ctx.pBufferEnd = data.end();
  ctx.pBufferStart = data.begin();
  
  uint32_t width = _UnpackInt(&ctx.pBuffer);
  uint32_t height = _UnpackInt(&ctx.pBuffer);
  ctx.image = atImage(atVec2I{ width, height });

  ctx.pDecompressed = (bool*)atAlloc(width * height);
  memset(ctx.pDecompressed, 0, width * height);

  while (ctx.pBuffer < ctx.pBufferEnd)
  {
    CompressionInstruction instruction = CI_Vertical;

    // Read the pixel and the instruction
    if (IS_PIXEL(*ctx.pBuffer))
      ctx.lastPixel = _UnpackColour(&ctx.pBuffer, &instruction);
        
    int64_t count = 1;
    // Read the count
    if (ctx.pBuffer < ctx.pBufferEnd && !IS_PIXEL(*ctx.pBuffer))
      count = _UnpackCount(&ctx.pBuffer, ctx.pBufferEnd);

    // Increment the cursor if the pixel has already been decompressed
    while (ctx.pDecompressed[ctx.cursor])
      ++ctx.cursor;

    // Execute the decompression instruction
    if (count > 1)
      _DecompressBlock(&ctx, instruction, count);
    else
      ctx.image[ctx.cursor++] = ctx.lastPixel;
  }

  atFree(ctx.pDecompressed);

  return std::move(ctx.image);
}
