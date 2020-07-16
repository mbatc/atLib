#ifndef atImageCompressor_h__
#define atImageCompressor_h__

#include "atImage.h"

// A simple near lossless image compression algorithm
// 
// The main concept is similar to how variable length unicode characters
// work. The least significant bit of each pixel is discard. In there place
// a compression instruction is stored. The least significant bit of the first byte
// of indicates if the following bytes is pixel data, or compression instruction
// arguments.
//
// If the least significant bit of a byte is 0 the next three bytes are 
// interpreted as a pixel.
//
// The least significant bit of the green and blue channels indicate the
// compression instruction.
//
// The main aim is to compress blocks of identical pixels by iterating
// horizontally, vertically or both.
//
// Lossy compression can be achieved by reducing the colour accuracy of the
// image and removing high frequency details - otherwise the algorithm can
// remain the same.
//
// The compressed block has the following structure. 
//
// [Pixel][Count][Pixel][Pixel][Count],
//   where count indicates the of times a pixel is to be repeated when
//   decompressing the image. If a count block is not present, the
//   previous pixel is only written once to the final image.

class atImageCompressor
{
public:
  static atVector<uint8_t> Compress(const atImage &image);
  static atImage Decompress(const atVector<uint8_t> &data);
};

#endif // atImageCompressor_h__
