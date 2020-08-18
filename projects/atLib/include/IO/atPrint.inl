
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

template<typename T> atString atPrint::Print(const T &object)
{
  return atToString(object);
}

template<typename T> atString atPrint::Float(const T &val)
{
  return atToString((double)val);
}

template<typename T> atString atPrint::Int(const T &val)
{
  return atToString((int64_t)val);
}

template<typename T> atString atPrint::Vector(const atVector2<T> &vec, const char *separator) { return atToString(vec.x) + separator + atToString(vec.y); }
template<typename T> atString atPrint::Vector(const atVector3<T> &vec, const char *separator) { return Vector(vec.xy()) + separator + atToString(vec.z); }
template<typename T> atString atPrint::Vector(const atVector4<T> &vec, const char *separator) { return Vector(vec.xyz()) + separator + atToString(vec.w); }

