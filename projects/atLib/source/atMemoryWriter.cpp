
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

#include "atMemoryWriter.h"

atMemoryWriter::atMemoryWriter() {}
atMemoryWriter::atMemoryWriter(atMemoryWriter &&move) : m_data(std::move(move.m_data)) {}
atMemoryWriter::atMemoryWriter(const atMemoryWriter &copy) : m_data(copy.m_data) {}
atMemoryWriter::~atMemoryWriter() {}

int64_t atMemoryWriter::Write(void *pData, const int64_t len)
{
  m_data.resize(atMax(m_pos + len, m_data.size()));
  memcpy(m_data.data() + m_pos, pData, len);
  m_pos += len;
  return len;
}

void atMemoryWriter::Clear()
{
  m_data.clear();
  m_pos = 0;
}

bool atMemoryWriter::Seek(const int64_t offset, const atFileSeek start)
{
  switch (start)
  {
  case atFS_Current: m_pos = m_pos + offset; break;
  case atFS_End: m_pos = m_data.size() + offset; break;
  case atFS_Start: m_pos = 0 + offset; break;
  case atFS_Invalid: default: return false;
  }

  m_pos = atMax(0, atMin(m_pos, m_data.size()));
  return true;
}

bool atMemoryWriter::operator!=(const atMemoryWriter &rhs) { return !(*this == rhs); }
bool atMemoryWriter::operator==(const atMemoryWriter &rhs) { return rhs.m_data.size() == m_data.size() && memcmp(rhs.m_data.data(), m_data.data(), m_data.size()) == 0; }
