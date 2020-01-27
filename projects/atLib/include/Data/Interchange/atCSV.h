#ifndef atCSV_h__
#define atCSV_h__

#include "atString.h"

class atCSV
{
public:
  atCSV() = default;

  atCSV(const atString &csv);

  atCSV(atCSV &&csv);
  atCSV(const atCSV &csv);

  atCSV& operator=(atCSV &&rhs);
  atCSV& operator=(const atCSV &rhs);


  bool Parse(const atString &csv);

  int64_t AsInt(const int64_t &row, const int64_t &column) const;
  bool AsBool(const int64_t &row, const int64_t &column) const;
  double AsFloat(const int64_t &row, const int64_t &column) const;
  atString AsString(const int64_t &row, const int64_t &column) const;

  // Set value will set or add the value
  void SetValue(const atString &value, const int64_t &row, const int64_t &col);

  // Template version of SetValue() for convenience. An atToString() must exist for type 'T'
  template<typename T> void SetValue(const T &val, const int64_t &row, const int64_t &col);

  // Add value will fail if the cell already exists
  bool AddValue(const atString &value, const int64_t &row, const int64_t &col);

  // Template version of AddValue() for convenience. An atToString() must exist for type 'T'
  template<typename T> bool AddValue(const T &val, const int64_t &row, const int64_t &col);

  atString* TryGetValue(const int64_t &row, const int64_t &col);
  const atString* TryGetValue(const int64_t &row, const int64_t &col) const;

  int64_t GetRowCount() const;
  int64_t GetColCount(const int64_t &row) const;

  friend atString atToString(const atCSV &csv);

protected:
  atVector<atVector<atString>> m_cells;
};

template<typename T> void atCSV::SetValue(const T &val, const int64_t &row, const int64_t &col) { SetValue(atToString(val), row, col); }
template<typename T> bool atCSV::AddValue(const T &val, const int64_t &row, const int64_t &col) { return AddValue(atToString(val), row, col); }

#endif // atCSV_h__
