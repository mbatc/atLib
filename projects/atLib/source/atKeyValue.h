
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

#ifndef _atKeyValue_h__
#define _atKeyValue_h__

template <class Key, class Val> class atKeyValue
{
public:
  atKeyValue(const Key &key, const Val &val)
    : m_key(key)
    , m_val(val)
  {}

  atKeyValue(const atKeyValue<Key, Val> &copy)
    : m_key(copy.m_key)
    , m_val(copy.m_val)
  {}

  atKeyValue(atKeyValue<Key, Val> &&move)
    : m_key(move.m_key)
    , m_val(move.m_val)
  {}

  bool compare(const atKeyValue<Key, Val> &rhs) { return rhs.m_key == m_key && rhs.m_val == m_val; }
  bool operator==(const atKeyValue<Key, Val> &rhs) { return compare(rhs); }

  const atKeyValue<Key, Val>& operator=(const atKeyValue<Key, Val> &rhs)
  {
    m_val = rhs.m_val;
    m_key = rhs.m_key;
    return *this;
  }

  Key m_key;
  Val m_val;
};

#endif