
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

template<typename T> atVector4<T> atColor::UnPackARGB(atCol col) { return atVector4<T>(atCOL_GETG(col), atCOL_GETB(col), atCOL_GETA(col), atCOL_GETR(col)); }
template<typename T> atVector4<T> atColor::UnPack(atCol col) { return atVector4<T>(atCOL_GETR(col), atCOL_GETG(col), atCOL_GETB(col), atCOL_GETA(col)); }
template<typename T> atCol atColor::Pack(const atVector4<T> &col) { return Pack(col.x, col.y, col.z, col.w); }
template<typename T> atCol atColor::Pack(const atVector3<T> &col) { return Pack(col.x, col.y, col.z, 255); }
template<typename T> atCol atColor::Pack(T r, T g, T b, T a) { return atCOL_PACK_RGBA(r, g, b, a); }
template<typename T> atCol atColor::PackARGB(const atVector4<T>& col) { return Pack(col.w, col.x, col.y, col.z); }
template<typename T> atCol atColor::PackARGB(const atVector3<T>& col) { return Pack(255, col.x, col.y, col.z); }
template<typename T> atCol atColor::PackARGB(T r, T g, T b, T a) { return Pack(a, r, g, b); }
