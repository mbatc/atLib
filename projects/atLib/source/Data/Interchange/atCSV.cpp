#include "atCSV.h"
#include "atScan.h"

atCSV::atCSV(const atString &csv) { Parse(csv); }
atCSV::atCSV(atCSV &&csv) { *this = std::move(csv); }
atCSV::atCSV(const atCSV &csv) { *this = csv; }

atCSV& atCSV::operator=(atCSV &&rhs)
{
  std::swap(m_cells, rhs.m_cells);
  return *this;
}

atCSV& atCSV::operator=(const atCSV &rhs)
{
  m_cells = rhs.m_cells;
  return *this;
}

bool atCSV::Parse(const atString &csv)
{
  *this = atCSV();

  if (csv.length() == 0)
    return false;

  atVector<atString> rows = csv.split("\r\n", true);
  for (const atString &row : rows)
  {
    atVector<atString> cols = row.split(',', false);

    m_cells.push_back(std::move(cols));
  }
  return false;
}

int64_t atCSV::AsInt(const int64_t &row, const int64_t &column) const
{
  const atString *pVal = TryGetValue(row, column);
  return pVal ? atScan::Int(*pVal) : 0;
}

bool atCSV::AsBool(const int64_t &row, const int64_t &column) const
{
  const atString *pVal = TryGetValue(row, column);
  return pVal ? atScan::Bool(*pVal) : 0;
}

double atCSV::AsFloat(const int64_t &row, const int64_t &column) const
{
  const atString *pVal = TryGetValue(row, column);
  return pVal ? atScan::Float(*pVal) : 0;
}

atString atCSV::AsString(const int64_t &row, const int64_t &column) const
{
  const atString *pVal = TryGetValue(row, column);
  return pVal ? *pVal : "";
}

void atCSV::SetValue(const atString &value, const int64_t &row, const int64_t &col)
{
  atString *pCell = TryGetValue(row, col);
  if (pCell)
    *pCell = value;
  else
    AddValue(value, row, col);
}

bool atCSV::AddValue(const atString &value, const int64_t &row, const int64_t &col)
{
  atString *pCell = TryGetValue(row, col);
  if (pCell && pCell->length() > 0)
    return false;

  if (!pCell)
  {
    m_cells.resize(atMax(m_cells.size(), row + 1));
    m_cells[row].resize(atMax(m_cells[row].size(), col + 1));
    pCell = TryGetValue(row, col);

    if (!pCell)
      return false;
  }

  *pCell = value;
  return true;
}

const atString* atCSV::TryGetValue(const int64_t &row, const int64_t &col) const { return col >= 0 && col < GetColCount(row) ? &m_cells[row][col] : nullptr; }
atString* atCSV::TryGetValue(const int64_t &row, const int64_t &col) { return col >= 0 && col < GetColCount(row) ? &m_cells[row][col] : nullptr; }
int64_t atCSV::GetRowCount() const { return m_cells.size(); }
int64_t atCSV::GetColCount(const int64_t &row) const { return row >= 0 && row < GetRowCount() ? m_cells[row].size() : 0; }

atString atToString(const atCSV &csv)
{
  atVector<atString> joinedRows;
  for (const atVector<atString> &row : csv.m_cells)
    joinedRows.push_back(atString::join(row, ",", false));
  return atString::join(joinedRows, "\n", false);
}
