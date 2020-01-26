
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
#include "atScan.h"

atDateTime::atDateTime() { Set(time(nullptr)); }
atDateTime::atDateTime(const int64_t time) { Set(time); }
atDateTime::atDateTime(tm *pData) { Set(mktime(pData)); }

bool atDateTime::Parse(const atString &datetime, const atDateTimeFmt &fmt)
{
  int64_t datetimeVals[atDTC_Count] = { 0 };
  int64_t lastPos = 0;
  int64_t curTimePos = 0;
  while (true)
  {
    int64_t nextPos = datetime.find_first_of("- :/\\", lastPos);
    datetimeVals[curTimePos++] = atScan::Int(datetime.c_str() + lastPos, nullptr, nextPos == AT_INVALID_INDEX ? -1 : nextPos - lastPos);
    if (nextPos == AT_INVALID_ID || curTimePos >= 6)
      break;
    lastPos = datetime.find_first_not("- :/\\", nextPos);
  }

  m_day = datetimeVals[fmt.compIdx[atDTC_Day] + (fmt.dateFirst ? 0 : 3)];
  m_month = datetimeVals[fmt.compIdx[atDTC_Month] + (fmt.dateFirst ? 0 : 3)];
  m_year = datetimeVals[fmt.compIdx[atDTC_Year] + (fmt.dateFirst ? 0 : 3)];
  m_hour = datetimeVals[fmt.compIdx[atDTC_Hour] + (fmt.dateFirst ? 3 : 0)];
  m_second = datetimeVals[fmt.compIdx[atDTC_Second] + (fmt.dateFirst ? 3 : 0)];
  m_min = datetimeVals[fmt.compIdx[atDTC_Minute] + (fmt.dateFirst ? 3 : 0)];
  return true;
}

atDateTime::atDateTime(const atString &datetime, const atDateTimeFmt &fmt) { Parse(datetime, fmt); }

atDateTime::atDateTime(const int64_t &day, const int64_t &month, const int64_t &year, const int64_t &hour, const int64_t &min, const int64_t &sec)
{
  m_day = day;
  m_month = month;
  m_year = year;
  m_hour = hour;
  m_min = min;
  m_second = sec;
}

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

bool atDateTime::operator>(const atDateTime &rhs) const { return to_time_t() > rhs.to_time_t(); }
bool atDateTime::operator<(const atDateTime &rhs) const { return to_time_t() < rhs.to_time_t(); }
bool atDateTime::operator==(const atDateTime &rhs) const { return to_time_t() == rhs.to_time_t(); }
bool atDateTime::operator>=(const atDateTime &rhs) const { return !(*this < rhs); }
bool atDateTime::operator<=(const atDateTime &rhs) const { return !(*this > rhs); }
bool atDateTime::operator!=(const atDateTime &rhs) const { return !(*this == rhs); }

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

atString atToString(const atDateTime &date)
{
  atString dateTime;
  atDateTimeFmt fmt = date.m_fmt;

  bool isDate = fmt.dateFirst;
  int64_t lastIsDate = -1;
  int64_t componentsUsed = 0;
  int64_t lastIdx = 0;

  while (componentsUsed < 6)
  {
    int64_t minIdx = INT64_MAX;
    int64_t nextComp = -1;
    for (int64_t i = isDate ? 0 : 3; i < (isDate ? 3 : atDTC_Count); ++i)
      if (fmt.compIdx[i] < minIdx)
      {
        nextComp = i;
        minIdx = fmt.compIdx[i];
      }

    if (nextComp == -1)
      return dateTime;

    fmt.compIdx[nextComp] = INT64_MAX;

    switch (nextComp)
    {
    case atDTC_Day: dateTime += date.m_day; break;
    case atDTC_Month: dateTime += date.m_month; break;
    case atDTC_Year: dateTime += date.m_year; break;
    case atDTC_Hour: dateTime += date.m_hour % (fmt.base24Time ? 24 : 12); break;
    case atDTC_Minute: dateTime += date.m_min; break;
    case atDTC_Second: dateTime += date.m_second; break;
    }

    ++componentsUsed;

    if (componentsUsed % 3 != 0)
    {
      dateTime += isDate ? fmt.dateSep : fmt.timeSep;
    }
    else
    {
      if (!fmt.base24Time && !isDate)
        dateTime += date.m_hour >= 12 ? " PM": " AM";

      dateTime += fmt.dateTimeSep;
      isDate = !isDate;
    }
  }

  return dateTime;
}
