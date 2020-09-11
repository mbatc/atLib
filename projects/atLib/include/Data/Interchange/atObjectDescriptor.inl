
template<typename T> inline atObjectDescriptor::atObjectDescriptor(const T &o) : atObjectDescriptor() { atSerialize(this, &o); }

template<typename T> inline atObjectDescriptor atObjectDescriptor::operator=(const T &o) { return Serialize(o); }

template<typename T>
inline atObjectDescriptor atObjectDescriptor::Serialize(const T &value)
{
  Clear();
  atSerialize(this, value);
  return *this;
}

template<typename T>
inline T atObjectDescriptor::Deserialize() const
{
  uint8_t buffer[sizeof(T)] = { 0 };
  Deserialize((T *)buffer);
  return std::move(*(T*)buffer);
}

template<typename T>
inline void atObjectDescriptor::Deserialize(T *pValue) const
{
  if (IsNull())
    return;
  atDeserialize(*this, pValue);
}
