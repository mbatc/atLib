
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

template<typename T> void atRenderableCore::SetChannel(const atString &name, const T &data, const atRenderable_ResourceType type)
{
  Resource &res = GetResource(name);
  res = Resource();
  res.count = 1;
  res.desc = atGetTypeDesc<T>();
  res.type = type;
  res.data.resize(res.desc.size);
  memcpy(res.data.data(), &data, (size_t)res.data.size());
}

template<typename T> void atRenderableCore::SetChannel(const atString &name, const atVector<T> &data, const atRenderable_ResourceType type)
{
  Resource &res = GetResource(name);
  res = Resource();
  res.desc = atGetTypeDesc<T>();
  res.count = data.size();
  res.type = type;
  res.data.resize(res.desc.size * res.count);
  memcpy(res.data.data(), data.data(), (size_t)res.data.size());
}

template<typename T> void atRenderableCore::SetChannel(const atString & name, const std::initializer_list<T>& list, const atRenderable_ResourceType type) { SetChannel(name, atVector<T>(list), type); }
