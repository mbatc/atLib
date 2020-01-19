
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

#include "atDateTime.h"
#include <time.h>

atDateTime::atDateTime() { Set(time(nullptr)); }
atDateTime::atDateTime(const int64_t time) { Set(time); }
atDateTime::atDateTime(tm *pData) { Set(mktime(pData)); }

int64_t atDateTime::to_time_t() const
{
  time_t rawtime;
  time(&rawtime);
  tm *pTimeinfo = localtime(&rawtime);
  pTimeinfo->tm_hour = (int)m_hour;
  pTimeinfo->tm_min = (int)m_min;
  pTimeinfo->tm_sec = (int)m_second;
  pTimeinfo->tm_mday = (int)m_day;
  pTimeinfo->tm_mon = (int)m_month - 1;
  pTimeinfo->tm_year = (int)m_year - 1900;
  return (int64_t)mktime(pTimeinfo);
}

void atDateTime::Set(const int64_t time)
{
  time_t myTime = time;
  tm data;
  localtime_s(&data, &myTime);

  m_day = data.tm_mday;
  m_year = data.tm_year + 1900;
  m_month = data.tm_mon + 1;

  m_min = data.tm_min;
  m_hour = data.tm_hour;
  m_second = data.tm_sec;
}
