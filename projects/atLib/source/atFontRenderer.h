
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

#ifndef atFontRenderer_h__
#define atFontRenderer_h__

#include "atFilename.h"
#include "atWindow.h"

class atFontRenderer
{
public:
  atFontRenderer() = delete;
  static void SetFont(const atFilename &font);
  static void Bake(const int64_t x, const int64_t y, const atString &text, const atVec2F &pivot = {0, 0});
  static void Draw(const atWindow &wnd);
  static atVec2I TextSize(const atString &text);
  static atVec4I TextRect(const int64_t x, const int64_t y, const atString &text, const atVec2F &pivot = { 0, 0 });

  static void PushColour(const atVec4F &color);
  static void PopColour(const int64_t count = 1);
  static void PushClipRect(const atVec4I &rect);
  static void PopClipRect(const int64_t count = 1);
};

#endif // atFontRenderer_h__
