#include "atSQL.h"
#include "atFileSystem.h"
  
#include "../../../3rdParty/sqlite3/include/sqlite3.h"

atSQL::atSQL(const atFilename &file)
{
  atFileSystem::CreateFolders(file.Directory());
  atRelAssert(sqlite3_open(file.c_str(), (sqlite3**)&m_pHandle) == SQLITE_OK, ("Failed to open SQL Database " + file.Path()).c_str());
}

atSQL::~atSQL()
{
  if (m_pHandle)
    sqlite3_close((sqlite3*)m_pHandle);
}

bool atSQL::IsThreadSafe() { return sqlite3_threadsafe() != 0; }

atSQLResult atSQL::Execute(const atString &query)
{
  sqlite3_stmt *pStatement;
  const char *pUnused;
  if (sqlite3_prepare_v2((sqlite3*)m_pHandle, query.c_str(), (int)query.length() + 1, &pStatement, &pUnused) != SQLITE_OK)
  {
    printf("Query '%s' failed with error:%s\n", query.c_str(), sqlite3_errmsg((sqlite3*)m_pHandle));
    return atSQLResult(0);
  }

  int64_t nColumns = sqlite3_column_count(pStatement);

  atVector<atString> colNames(nColumns);
  for (int64_t col = 0; col < nColumns; ++col)
    colNames.push_back(sqlite3_column_name(pStatement, (int)col));

  atSQLResult result(colNames);
  while (true)
  {
    int errorCode = sqlite3_step(pStatement);
    if (errorCode == SQLITE_ROW)
    {
      result.NextRow();
      for (int64_t col = 0; col < nColumns; ++col)
        result.SetColumn(sqlite3_column_value(pStatement, (int)col), col);
    }
    else if (errorCode != SQLITE_OK)
    {
      if (errorCode != SQLITE_DONE)
        printf("(%d): %s\n", errorCode, sqlite3_errmsg((sqlite3*)m_pHandle));

      break;
    }
  }

  sqlite3_finalize(pStatement);
  return result;
}

atVector<uint8_t> atSQLResult::AsBlob(const int64_t &row, const int64_t &col)
{
  int64_t len = sqlite3_value_bytes((sqlite3_value*)GetValue(row, col));
  uint8_t *pBlock = (uint8_t*)sqlite3_value_blob((sqlite3_value*)GetValue(row, col));
  return atVector<uint8_t>(pBlock, len);
}

atType atSQLResult::GetDefaultType(const int64_t &row, const int64_t &col)
{
  if (!IsValid(row, col))
    return atType_Unknown;

  switch (sqlite3_value_type((sqlite3_value*)GetValue(row, col)))
  {
  case SQLITE_INTEGER: return atType_Int64;
  case SQLITE_FLOAT: return atType_Float64;
  case SQLITE_BLOB: return atType_Uint8;
  case SQLITE_TEXT: return atType_Uint8;
  case SQLITE_NULL: return atType_Unknown;
  }

  return atType_Unknown;
}

void atSQLResult::SetColumn(void *pValue, int64_t col)
{
  void *&pV = GetValue(m_nRows - 1, col);
  if (pV)
    sqlite3_value_free((sqlite3_value*)pV);
  pV = sqlite3_value_dup((sqlite3_value*)pValue);
}

atSQLResult::atSQLResult(const atVector<atString> &columns)
  : m_nRows(0)
{
  m_columns = columns;
}

atSQLResult::atSQLResult(const atSQLResult &copy)
{
  m_columns = copy.m_columns;
  m_nRows = copy.m_nRows;
  m_values.reserve(copy.m_values.size());
  for (void *pValue : copy.m_values)
    m_values.push_back(sqlite3_value_dup((sqlite3_value*)pValue));
}

atSQLResult::atSQLResult(atSQLResult &&move)
{
  m_columns = std::move(move.m_columns);
  m_values = std::move(move.m_values);
  m_nRows = move.m_nRows;
  move.m_values.clear();
  move.m_columns.clear();
  move.m_nRows = 0;
}

atSQLResult::~atSQLResult()
{
  for (void *pValue : m_values)
    sqlite3_value_free((sqlite3_value*)pValue);
}

int64_t atSQLResult::AsInt(const int64_t &row, const int64_t &col) { return IsValid(row, col) ? sqlite3_value_int64((sqlite3_value*)GetValue(row, col)) : 0; }
double atSQLResult::AsFloat(const int64_t &row, const int64_t &col) { return IsValid(row, col) ? sqlite3_value_double((sqlite3_value*)GetValue(row, col)) : 0.0; }
atString atSQLResult::AsString(const int64_t &row, const int64_t &col) { return IsValid(row, col) ? (const char*)sqlite3_value_text((sqlite3_value*)GetValue(row, col)) : ""; }
bool atSQLResult::IsValid(const int64_t &row, const int64_t &column) { return row >= 0 && row < RowCount() && column >= 0 && column < ColumnCount() && GetValue(row, column) != nullptr; }
void*& atSQLResult::GetValue(const int64_t &row, const int64_t &column) { return m_values[row * ColumnCount() + column]; }
int64_t atSQLResult::RowCount() { return m_nRows; }
int64_t atSQLResult::ColumnCount() { return m_columns.size(); }
atString atSQLResult::ColumnName(const int64_t &col) { return col < 0 || col >= m_columns.size() ? "" : m_columns[col]; }
void atSQLResult::NextRow() { m_values.resize(++m_nRows * m_columns.size()); }
