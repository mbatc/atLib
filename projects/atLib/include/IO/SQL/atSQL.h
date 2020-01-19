#ifndef atSQL_h__
#define atSQL_h__

#include "atKeyValue.h"
#include "atFilename.h"

class atSQL;

class atSQLResult
{
  friend atSQL;

  atSQLResult(const atVector<atString> &columns);

public:
  int64_t AsInt(const int64_t &row, const int64_t &col);
  double AsFloat(const int64_t &row, const int64_t &col);
  atString AsString(const int64_t &row, const int64_t &col);
  atVector<uint8_t> AsBlob(const int64_t &row, const int64_t &col);

  atType GetDefaultType(const int64_t &row, const int64_t &col);
  int64_t RowCount();
  int64_t ColumnCount();

  atString ColumnName(const int64_t &col);

protected:
  int64_t m_nRows;
  atVector<atString> m_columns;
  bool IsValid(const int64_t &row, const int64_t &column);
  void*& GetValue(const int64_t &row, const int64_t &column);
  void SetColumn(void *pValue, int64_t col);
  void NextRow();

  atVector<void*> m_values;
};

class atSQL
{
public:
  atSQL(const atFilename &file);
  ~atSQL();

  atVector<atKeyValue<atString, atType>> GetTableDescription(const atString &name);

  bool HasTable(const atString &name);
  void AddTable(const atString &name, const atVector<atKeyValue<atString, atType>> &tableDesc);

  atSQLResult Execute(const atString &query);

protected:
  void *m_pHandle;
};

#endif // atSQL_h__
