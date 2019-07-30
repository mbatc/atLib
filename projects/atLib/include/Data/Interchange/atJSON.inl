
template<typename T> inline void atJSON::SetValue(const T &val, const bool &isString)
{
  MakeValue(isString);
  *m_pValue = atString(val);
}

template<typename T> inline T atJSON::GetValueAs() const { return IsValue() || IsString() ? T(*m_pValue) : T(); }