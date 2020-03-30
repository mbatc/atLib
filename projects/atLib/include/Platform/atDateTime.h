
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

#ifndef atDateTime_h__
#define atDateTime_h__

#include "atString.h"

enum atDateTimeComponent
{
  atDTC_Day,
  atDTC_Month,
  atDTC_Year,
  atDTC_Hour,
  atDTC_Minute,
  atDTC_Second,
  atDTC_Count,
};

struct atDateTimeFmt
{
  atDateTimeFmt()
  {
    for (int64_t i = 0; i < atDTC_Count; ++i)
      compIdx[i] = i % 3;
  }

  int64_t compIdx[atDTC_Count];
  bool dateFirst = true;
  bool base24Time = false;
  char timeSep = ':';
  char dateSep = '/';
  char dateTimeSep = ' ';
};

class atDateTime
{
public:
  // Create a Data Time object with the current time
  atDateTime();
  atDateTime(const atDateTime &o);
  atDateTime(atDateTime &&o);
  atDateTime& operator=(const atDateTime &o);
  atDateTime& operator=(atDateTime &&o);

  // Construct a datetime from a string.
  // Use the 'fmt' parameter to specify the order of date/time components
  // Note: the only thing used from the 'fmt' struct on read is 'compIdx' and 'dateFirst' for the ordering of each component
  atDateTime(const atString &datetime, const atDateTimeFmt &fmt = atDateTimeFmt());

  // Construct from individual components
  atDateTime(const int64_t &day, const int64_t &month, const int64_t &year, const int64_t &hour, const int64_t &min, const int64_t &sec);

  // Create a Data Time object representing [time]
  atDateTime(const int64_t time);
  atDateTime(tm *pData);
  ~atDateTime();

  bool Parse(const atString &datetime, const atDateTimeFmt &fmt = atDateTimeFmt());
  
  void SetYear(int64_t year);
  void SetMonth(int64_t mon);
  void SetDay(int64_t day);
  void SetHour(int64_t hr);
  void SetMin(int64_t min);
  void SetSecond(int64_t sec);

  const int64_t& GetYear() const;
  const int64_t& GetMonth() const;
  const int64_t& GetDay() const;
  const int64_t& GetHour() const;
  const int64_t& GetMin() const;
  const int64_t& GetSecond() const;

  const int64_t& to_time_t() const;

  atDateTimeFmt m_fmt; // Used when converting to a string

  friend atString atToString(const atDateTime &date);

  bool operator>(const atDateTime &rhs) const;
  bool operator<(const atDateTime &rhs) const;
  bool operator>=(const atDateTime &rhs) const;
  bool operator<=(const atDateTime &rhs) const;
  bool operator==(const atDateTime &rhs) const;
  bool operator!=(const atDateTime &rhs) const;

protected:
  int64_t m_year;
  int64_t m_month;
  int64_t m_day;

  int64_t m_hour;
  int64_t m_min;
  int64_t m_second;

  struct FullTime
  {
    bool dirty = false;
    int64_t val = 0;
  } *m_pFullTime = atNew<FullTime>();

  void Set(const int64_t time);
};

#endif // atDateTime_h__
