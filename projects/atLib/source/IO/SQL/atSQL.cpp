#include "atSQL.h"
#include "atFileSystem.h"
#include "../../../3rdParty/sqlite3/include/sqlite3.h"

atSQL::atSQL(const atFilename &file)
{
  atFileSystem::CreateFolders(file.Directory());
  if (sqlite3_open(file.c_str(), (sqlite3**)&m_pHandle) != SQLITE_OK)
    m_pHandle = nullptr;
}

atSQL::~atSQL()
{
  if (m_pHandle)
    sqlite3_close((sqlite3*)m_pHandle);
}

bool atSQL::IsThreadSafe() { return sqlite3_threadsafe() != 0; }
bool atSQL::IsOpen() const { return m_pHandle != nullptr; }

atSQLResult atSQL::Execute(const atString &query) const
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

atVector<atString> atSQL::Tables() const
{
  atSQLResult tables = Execute("SELECT name FROM sqlite_master WHERE type=='table' AND name NOT LIKE 'sqlite_%'");
  atVector<atString> names;
  for (int64_t i = 0; i < tables.RowCount(); ++i)
    names.push_back(tables.AsString(i, 0));
  return names;;
}

atVector<atString> atSQL::Columns(const atString &tableName) const
{
  atVector<atString> names;
  for (const atString &colSql : ColumnSQL(tableName))
    names.push_back(colSql.substr(0, colSql.find_first_of(atString::Whitespace())));
  return names;
}

atVector<atString> atSQL::ColumnSQL(const atString &tableName) const
{
  atSQLResult table = Execute("SELECT sql FROM sqlite_master WHERE type=='table' AND name=='" + tableName + "'");
  if (table.Empty())
    return {};

  atVector<atString> rows;
  atString tableSql = table.AsString(0, 0);
  rows = tableSql.substr(tableSql.find('(') + 1, tableSql.find_last_of(')')).split(',');
  for (atString &rowRaw : rows)
    rowRaw = rowRaw.trim();
  return rows;
}

atVector<uint8_t> atSQLResult::AsBlob(const int64_t &row, const int64_t &col) const
{
  int64_t len = sqlite3_value_bytes((sqlite3_value*)GetValue(row, col));
  uint8_t *pBlock = (uint8_t*)sqlite3_value_blob((sqlite3_value*)GetValue(row, col));
  return atVector<uint8_t>(pBlock, len);
}

atType atSQLResult::GetDefaultType(const int64_t &row, const int64_t &col) const
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

atSQLResult::atSQLResult(const atSQLResult &copy) { *this = copy; }
atSQLResult::atSQLResult(atSQLResult &&move) { *this = std::move(move); }

atSQLResult::~atSQLResult()
{
  for (void *pValue : m_values)
    sqlite3_value_free((sqlite3_value*)pValue);
}

atSQLResult &atSQLResult::operator=(const atSQLResult &copy)
{
  this->~atSQLResult();
  m_columns = copy.m_columns;
  m_nRows = copy.m_nRows;
  m_values.reserve(copy.m_values.size());
  for (void *pValue : copy.m_values)
    m_values.push_back(sqlite3_value_dup((sqlite3_value *)pValue));
  return *this;
}

atSQLResult &atSQLResult::operator=(atSQLResult &&move)
{
  this->~atSQLResult();
  m_columns = std::move(move.m_columns);
  m_values = std::move(move.m_values);
  m_nRows = move.m_nRows;
  move.m_values.clear();
  move.m_columns.clear();
  move.m_nRows = 0;
  return *this;
}

int64_t atSQLResult::FindColumn(const atString &columnName) const
{
  for (int64_t i = 0; i < m_columns.size(); ++i)
    if (m_columns[i] == columnName)
      return i;
  return -1;
}

atVector<int64_t> atSQLResult::FindColumns(const atVector<atString> &columnNames) const
{
  atVector<int64_t> indices(columnNames.size());
  for (const atString &name : columnNames)
    indices.push_back(FindColumn(name));
  return indices;
}

bool atSQLResult::Empty() const { return m_values.size() == 0; }

int64_t atSQLResult::AsInt(const int64_t &row, const int64_t &col) const { return IsValid(row, col) ? sqlite3_value_int64((sqlite3_value*)GetValue(row, col)) : 0; }
double atSQLResult::AsFloat(const int64_t &row, const int64_t &col) const { return IsValid(row, col) ? sqlite3_value_double((sqlite3_value*)GetValue(row, col)) : 0.0; }
atString atSQLResult::AsString(const int64_t &row, const int64_t &col) const { return IsValid(row, col) ? (const char*)sqlite3_value_text((sqlite3_value*)GetValue(row, col)) : ""; }
bool atSQLResult::IsValid(const int64_t &row, const int64_t &column) const { return row >= 0 && row < RowCount() && column >= 0 && column < ColumnCount() && GetValue(row, column) != nullptr; }
void * const &atSQLResult::GetValue(const int64_t &row, const int64_t &column) const { return m_values[row * ColumnCount() + column]; }
void *&atSQLResult::GetValue(const int64_t &row, const int64_t &column) { return m_values[row * ColumnCount() + column]; }
int64_t atSQLResult::RowCount() const { return m_nRows; }
int64_t atSQLResult::ColumnCount() const { return m_columns.size(); }
atString atSQLResult::ColumnName(const int64_t &col) const { return col < 0 || col >= m_columns.size() ? "" : m_columns[col]; }

void atSQLResult::Print() const
{
  for (int64_t row = 0; row < RowCount(); ++row)
  {
    for (int64_t col = 0; col < ColumnCount(); ++col)
      printf("%s\t", AsString(row, col).c_str());
    printf("\n");
  }
}

void atSQLResult::NextRow() { m_values.resize(++m_nRows * m_columns.size()); }
