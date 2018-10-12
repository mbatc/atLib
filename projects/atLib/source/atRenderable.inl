
template<typename T> void atRenderable::SetChannel(const atString &name, const T &data, const atRenderable_ResourceType type)
{
  Resource &res = GetResource(name);
  res = Resource();
  res.count = 1;
  res.desc = atGetTypeDesc<T>();
  res.type = type;
  res.data.resize(res.desc.size);
  memcpy(res.data.data(), &data, res.data.size());
}

template<typename T> void atRenderable::SetChannel(const atString &name, const atVector<T> &data, const atRenderable_ResourceType type)
{
  Resource &res = GetResource(name);
  res = Resource();
  res.desc = atGetTypeDesc<T>();
  res.count = data.size();
  res.type = type;
  res.data.resize(res.desc.size * res.count);
  memcpy(res.data.data(), data.data(), res.data.size());
}

template<typename T> void atRenderable::SetChannel(const atString &name, const atStringBasic<T>& data, const atRenderable_ResourceType type)
{
  Resource &res = GetResource(name);
  res = Resource();
  res.desc = atGetTypeDesc<T>();
  res.count = data.length() + 1;
  res.type = type;
  res.data.resize(res.count * res.desc.size);
  memcpy(res.data.data(), data.c_str(), res.data.size());
}

template<typename T> void atRenderable::SetChannel(const atString & name, const std::initializer_list<T>& list, const atRenderable_ResourceType type) { SetChannel(name, atVector<T>(list), type); }
