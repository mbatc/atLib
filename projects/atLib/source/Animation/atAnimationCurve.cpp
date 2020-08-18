#include "atAnimationCurve.h"

int64_t atAnimationCurve::GetKeyCount() const { return m_keys.size(); }

const atAnimationKey& atAnimationCurve::GetKey(const int64_t &index) const { return m_keys[index].m_val; }
atNanoSeconds atAnimationCurve::GetKeyTime(const int64_t &index) const { return m_keys[index].m_key; }

int64_t atAnimationCurve::SetKeyTime(const int64_t &index, const atNanoSeconds &time)
{
  if (index < 0 || index >= m_keys.size())
    return -1;
  if (m_keys[index].m_key == time)
    return index;
  atAnimationKey key = GetKey(index);
  Remove(index);
  return SetKey(time, key);
}

void atAnimationCurve::SetKey(const int64_t &index, const atAnimationKey &key) { m_keys[index].m_val = key; }

int64_t atAnimationCurve::SetKey(const atNanoSeconds &time, const atAnimationKey &key)
{
  int64_t idx = Find(time);
  atKeyValue<atNanoSeconds, atAnimationKey> kvp(time, key);

  if (idx == -1)
  {
    idx = m_keys.size();
    m_keys.insert(0, kvp);
  }
  else if (m_keys[idx].m_key != time)
  {
    idx += 1;
    m_keys.insert(idx, kvp);
  }
  else
  {
    m_keys[idx] = kvp;
  }

  return idx;
}

double atAnimationCurve::Evaluate(const atNanoSeconds &time, int64_t *pLastIndex) const
{
  int64_t firstIdx = Find(time, pLastIndex);
  if (firstIdx == -1)
    return m_keys.size() == 0 ? m_defaultValue : m_keys[0].m_val.GetValue();

  int64_t secondIdx = firstIdx + 1;

  const atAnimationKey *pFirst = &m_keys[firstIdx].m_val;
  const atAnimationKey *pSecond = secondIdx < m_keys.size() ? &m_keys[secondIdx].m_val : nullptr;
  
  if (pSecond)
  {
    atNanoSeconds keySpan = m_keys[secondIdx].m_key - m_keys[firstIdx].m_key;
    atNanoSeconds t = time - m_keys[firstIdx].m_key;
    return pFirst->Interpolate(*pSecond, t.ToNanof() / keySpan.ToNano());
  }

  return pFirst->GetValue();
}

int64_t atAnimationCurve::Find(const atNanoSeconds &time, int64_t *pLastIndex) const
{
  int64_t first = 0;
  if (pLastIndex)
  {
    int64_t val = *pLastIndex;
    if (val >= 0 && val < m_keys.size() && m_keys[val].m_key.Get() < time.Get())
      first = val;
  }

  for (int64_t i = first; i < m_keys.size(); ++i)
    if (m_keys[i].m_key.Get() <= time.Get() && ((i + 1) >= m_keys.size() || m_keys[i + 1].m_key.Get() > time.Get()))
      return i;

  return -1;
}

bool atAnimationCurve::Remove(const int64_t &index)
{
  if (index < 0 || index >= m_keys.size())
    return false;
  m_keys.erase(index);
  return true;
}

void atAnimationCurve::SetDefaultValue(const double &value)
{
  m_defaultValue = value;
}
