
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

atImage::atImage() {}
atImage::atImage(const atFilename &file) { m_pixels = std::move(atImageHelper::LoadImage(file, &m_size)); }
atImage::atImage(const atVector<atCol> &data, const atVec2I &size) : m_pixels(data), m_size(size) {}
atVector<atCol>& atImage::Pixels() { return m_pixels; }
const atVector<atCol>& atImage::Pixels() const { return m_pixels; }
const atVec2I & atImage::Size() const { return m_size; }
const int64_t atImage::Width() const { return m_size.x; }
const int64_t atImage::Height() const { return m_size.y; }

void atImage::Resize(const atVec2I & size) {}
