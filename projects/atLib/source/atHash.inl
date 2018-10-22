template <typename T> int64_t atHash::Hash(const T &o)
{
  writer.Clear();
  writer.Write(o);
  return atHash::Hash(writer);
}