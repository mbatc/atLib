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
  atSQLResult() = default;
  atSQLResult(const atSQLResult &copy);
  atSQLResult(atSQLResult &&move);
  ~atSQLResult();

  atSQLResult& operator=(const atSQLResult &copy);
  atSQLResult& operator=(atSQLResult &&move);

  bool Empty() const;

  int64_t AsInt(const int64_t &row, const int64_t &col) const;
  double AsFloat(const int64_t &row, const int64_t &col) const;
  atString AsString(const int64_t &row, const int64_t &col) const;
  atVector<uint8_t> AsBlob(const int64_t &row, const int64_t &col) const;

  atType GetDefaultType(const int64_t &row, const int64_t &col) const;
  int64_t RowCount() const;
  int64_t ColumnCount() const;

  atString ColumnName(const int64_t &col) const;

  // Return the index of the columns that correspond to the given name
  int64_t FindColumn(const atString &columnName) const;

  // Return the indices of the columns that correspond to the names given
  atVector<int64_t> FindColumns(const atVector<atString> &columnNames) const;

  // Helper function to print the result to the console
  void Print() const;

protected:
  int64_t m_nRows;
  atVector<atString> m_columns;
  bool IsValid(const int64_t &row, const int64_t &column) const;
  void *&GetValue(const int64_t &row, const int64_t &column);
  void * const &GetValue(const int64_t &row, const int64_t &column) const;
  void SetColumn(void *pValue, int64_t col);
  void NextRow();

  atVector<void*> m_values;
};

class atSQL
{
public:
  atSQL(const atFilename &file);
  atSQL(atSQL &&o);
  atSQL(const atSQL &o);
  ~atSQL();

  // Check if the SqlLite implementation is thread-safe
  static bool IsThreadSafe();

  // Check if the data base file was opened successfully
  bool IsOpen() const;

  // Execute a database query
  atSQLResult Execute(const atString &query) const;

  // Get the names of the tables in the database
  atVector<atString> Tables() const;

  // Get the names of the columns in the specified table
  atVector<atString> Columns(const atString &tableName) const;

  const atFilename& GetPath() const;

protected:
  atVector<atString> ColumnSQL(const atString &tableName) const;

  void *m_pHandle = nullptr;
  atFilename m_path;
};

#endif // atSQL_h__
